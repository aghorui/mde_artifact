#include "llvm/IR/Function.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/ReplaceConstant.h"
#include "llvm/IR/Instructions.h"

#include <stack>
#include <map>

using namespace llvm;

namespace {

bool flattenExpressionList(llvm::Instruction *inst) {
	using namespace llvm;

	bool changed = false;
	std::stack<llvm::Instruction *> stack;

	stack.push(inst);

	while (stack.size() > 0) {
		Instruction *curr = stack.top();
		stack.pop();

		// PHI nodes require special handling in this case, because of multiple
		// Possibilities of incoming values being used in the constant expression.
		// We have to "hoist" the decomposed constexpr upwards so that we are
		// able to get the value of the phi instruction.
		//
		// In some cases, multiple operands of the phi instruction may point to
		// the same incoming block but with different constant expressions.
		// For that we need to make sure to not end up making duplicate
		// instructions.
		if (isa<PHINode>(curr)) {
			std::map<std::pair<BasicBlock *, Value *>, Value *> dedup;
			PHINode *phi = dyn_cast<PHINode>(curr);

			for (unsigned int i = 0; i < phi->getNumIncomingValues(); i++) {
				Value *oper = phi->getIncomingValue(i);
				BasicBlock *pred = phi->getIncomingBlock(i);

				if (!isa<ConstantExpr>(oper)) {
					continue;
				}

				changed = true;
				ConstantExpr *expr = dyn_cast<ConstantExpr>(oper);

				Value *finalIncoming = nullptr;
				auto cursor = dedup.find({pred, expr});

				if (cursor != dedup.end()) {
					finalIncoming = cursor->second;
				} else {
					Instruction *newInst = expr->getAsInstruction();
					newInst->insertBefore(pred->getTerminator());
					finalIncoming = newInst;
					dedup[{pred, expr}] = finalIncoming;
					stack.push(newInst);
				}

				phi->setIncomingValue(i, finalIncoming);
			}


		} else {
			for (unsigned int i = 0; i < curr->getNumOperands(); i++) {
				Value *oper = curr->getOperand(i);

				if (!isa<ConstantExpr>(oper)) {
					continue;
				}

				changed = true;
				ConstantExpr *expr = dyn_cast<ConstantExpr>(oper);
				Instruction *newInst = expr->getAsInstruction();
				newInst->insertBefore(curr);

				curr->setOperand(i, newInst);
				stack.push(newInst);
			}
		}

	}

	return changed;
}
struct FlattenConstExprPass : public PassInfoMixin<FlattenConstExprPass> {
	PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
		// simple example action
		for (auto &BB : F) {
			for (auto &inst : BB) {
				flattenExpressionList(&inst);
			}
		}
		return PreservedAnalyses::none();
	}
};

}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
	return {
		LLVM_PLUGIN_API_VERSION,
		"FlattenConstExprPass",
		"v0.1",
		[](PassBuilder &PB) {
			PB.registerPipelineParsingCallback(
				[](StringRef Name, FunctionPassManager &FPM,
				   ArrayRef<PassBuilder::PipelineElement>) {
					if (Name == "flatten-constexpr") {
						FPM.addPass(FlattenConstExprPass());
						return true;
					}
					return false;
				});
		}
	};
}