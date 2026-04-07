#ifndef LFCPA_COMMON_ALL_H
#define LFCPA_COMMON_ALL_H
#include "slim/IR.h"

#include <cstddef>
#include <llvm-14/llvm/Support/raw_ostream.h>
#include <sstream>


extern size_t STAT_operation_count;

extern size_t DBG_total_insts;


inline bool compareIndices(std::vector<SLIMOperand *> ipVec1, std::vector<SLIMOperand *> ipVec2) {
	if (ipVec1.size() != ipVec2.size())
		return false;

	for (int i = 0; i < ipVec1.size(); i++) {
		if (ipVec1[i]->getValue() != ipVec2[i]->getValue())
			return false;
	}
	return true;
}

inline bool compareOperands(SLIMOperand *a1, SLIMOperand *a2) {
	if (a1 == a2) {
		return true;
	}

	if (a1->isDynamicAllocationType() and a2->isDynamicAllocationType()) {
		// ignore indices
		if (a1->getValue() == a2->getValue() and compareIndices(a1->getIndexVector(), a2->getIndexVector())) {
			return true;
		}
	} else if (a1->isArrayElement() and a2->isArrayElement()) {
		llvm::Value *v1 = a1->getValueOfArray();
		llvm::Value *v2 = a2->getValueOfArray();
		if (v1 == v2) {
			return true;
		}
	} else {
		if (a1->getValue() == a2->getValue() and compareIndices(a1->getIndexVector(), a2->getIndexVector())) {
			return true;
		}
	}
	return false;
}

struct SLIMOperandLess {
	bool operator()(SLIMOperand *a, SLIMOperand *b) const {
		if (!compareOperands(a, b)) {
			return a < b;
		} else {
			return false;
		}
	}
};

struct SLIMOperandHash {
	std::size_t operator()(const SLIMOperand *a) const {
		if (!a)
			return 0;
		llvm::Value *v = const_cast<SLIMOperand *>(a)->getValue();
		return std::hash<const llvm::Value *>()(v);
	}
};

struct SLIMOperandEqual {
	bool operator()(SLIMOperand *a, SLIMOperand *b) const {
		return compareOperands(a, b);
	}
};

struct SLIMOperandPrinter {
	std::string operator()(SLIMOperand *a) const {
		std::stringstream s;
		s << a;
		return s.str();
		// return a->hasName() ? a->getName().str() : "<UNNAMED>";
	}
};
#endif