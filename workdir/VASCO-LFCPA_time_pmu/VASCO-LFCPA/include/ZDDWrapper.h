#ifndef ZDD_WRAPPER_H
#define ZDD_WRAPPER_H
#include "CommonAll.h"
#include "slim/IR.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <set>
#include <sstream>
#include <math.h>
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <cudd.h>

class ZDD {
protected:
	static DdManager* global_mgr;
	DdNode* node;

public:
	ZDD(DdNode* n) : node(n) {
		assert(node != nullptr);
		Cudd_Ref(node);
	}

	ZDD() {
		node = Cudd_ReadZero(global_mgr);
		Cudd_Ref(node);
	}

	~ZDD() {
		if (node != nullptr)
			Cudd_RecursiveDerefZdd(global_mgr, node);
	}

	ZDD(const ZDD& other) : node(other.node) {
		if (node != nullptr)
			Cudd_Ref(node);
	}

	template <typename Iterator>
	ZDD(Iterator begin, Iterator end) {
		node = create_from_iter(begin, end);
	}

	ZDD(std::initializer_list<int> l) {
		node = create_from_iter(l.begin(), l.end());
	}

	ZDD(int i) {
		// Cudd_zddChange "toggles" the variable on.
		node = Cudd_zddChange(global_mgr, Cudd_ReadOne(global_mgr), i);
		if (!node) {
			throw std::invalid_argument(std::string("Invalid integer value: ") + std::to_string(i));
		}
		Cudd_Ref(node);
	}

	// Iterator Constructor: Creates a family of DISJOINT singleton sets
	template <typename Iterator>
	static DdNode *create_from_iter(Iterator begin, Iterator end) {
		// 1. Start with the Empty Family (Zero)
		// This represents a container with NO sets in it.
		DdNode* temp = Cudd_ReadZero(global_mgr);
		Cudd_Ref(temp);

		for (Iterator it = begin; it != end; ++it) {
			int index = *it;

			// Cudd_zddChange(One, index) "toggles" the presence of the variable
			// index in all subsets. We are doing this toggling with a set with
			// the empty set.
			DdNode* var = Cudd_zddChange(global_mgr, Cudd_ReadOne(global_mgr), index);
			Cudd_Ref(var);

			DdNode* next = Cudd_zddUnion(global_mgr, temp, var);
			Cudd_Ref(next);

			Cudd_RecursiveDerefZdd(global_mgr, temp); // Release the old accumulator
			Cudd_RecursiveDerefZdd(global_mgr, var);  // Release the variable node

			temp = next;
		}

		return temp;
	}

	size_t size() const {
		return (size_t)Cudd_zddCount(global_mgr, node);
	}

	bool is_empty() const {
		return node == Cudd_ReadZero(global_mgr);
	}

	bool contains(int index) const {
		DdNode* var = Cudd_zddIthVar(global_mgr, index);
		Cudd_Ref(var);

		DdNode* intersection = Cudd_zddIntersect(global_mgr, node, var);
		Cudd_Ref(intersection);

		bool result = (intersection != Cudd_ReadZero(global_mgr));

		Cudd_RecursiveDerefZdd(global_mgr, intersection);
		Cudd_RecursiveDerefZdd(global_mgr, var);
		return result;
	}

	class iterator {
		std::vector<DdNode*> stack;
		std::unordered_set<DdNode*> visited;
		std::set<int> yielded_indices;
		int current_val;

		void advance() {
			while (!stack.empty()) {
				DdNode* current = stack.back();
				stack.pop_back();
				current = Cudd_Regular(current);

				if (Cudd_IsConstant(current) || visited.count(current)) {
					continue;
				}
				visited.insert(current);

				int index = Cudd_NodeReadIndex(current);
				stack.push_back(Cudd_E(current));
				stack.push_back(Cudd_T(current));

				if (yielded_indices.find(index) == yielded_indices.end()) {
					yielded_indices.insert(index);
					current_val = index;
					return;
				}
			}
			current_val = -1; // Sentinel
			stack.clear();
		}

	public:
		using iterator_category = std::input_iterator_tag;
		using value_type = int;
		using difference_type = std::ptrdiff_t;
		using pointer = const int*;
		using reference = const int&;

		iterator() : current_val(-1) {}

		iterator(DdNode* root) {
			if (root != nullptr) {
				stack.push_back(root);
				advance();
			} else {
				current_val = -1;
			}
		}

		reference operator*() const {
			return current_val;
		}

		pointer operator->() const {
			return &current_val;
		}

		iterator& operator++() {
			advance();
			return *this;
		}

		iterator operator++(int) {
			iterator tmp = *this;
			advance();
			return tmp;
		}

		bool operator==(const iterator& other) const {
			return
				current_val == other.current_val &&
				stack.empty() == other.stack.empty();
		}

		bool operator!=(const iterator& other) const {
			return !(*this == other);
		}
	};

	iterator begin() const { return iterator(node); }
	iterator end() const { return iterator(); }

	std::set<int> get_vars() const {
		std::set<int> distinct_indices;
		for (int idx : *this) {
			distinct_indices.insert(idx);
		}
		return distinct_indices;
	}

	// Assignment operator
	ZDD& operator=(const ZDD& other) {
		if (this != &other) {
			if (node != nullptr)
				Cudd_RecursiveDerefZdd(global_mgr, node);
			global_mgr = other.global_mgr;
			node = other.node;
			if (node != nullptr)
				Cudd_Ref(node);
		}
		return *this;
	}

	// CUDD node ptrs are canonical.
	bool operator==(const ZDD& other) const {
		return node == other.node;
	}

	// CUDD node ptrs are canonical.
	bool operator<(const ZDD& other) const {
		return node < other.node;
	}

	ZDD set_union(const ZDD& other) const {
		DdNode* res = Cudd_zddUnion(global_mgr, node, other.node);
		return ZDD(res);
	}

	ZDD set_intersection(const ZDD& other) const {
		DdNode* res = Cudd_zddIntersect(global_mgr, node, other.node);
		return ZDD(res);
	}

	ZDD set_difference(const ZDD& other) const {
		DdNode* res = Cudd_zddDiff(global_mgr, node, other.node);
		return ZDD(res);
	}

	ZDD insert_single(int i) const {
		ZDD result = this->set_union(ZDD(i));
		assert(result.size() > 0);
		return result;
	}

	ZDD remove_single(int i) const {
		return this->set_difference(ZDD(i));
	}

	void dump_dot(const std::string& filename) const {
		assert(node);

		FILE* fp = fopen(filename.c_str(), "w");
		if (fp == nullptr) {
			std::cerr << "Error opening file: " << filename << std::endl;
			return;
		}

		// Cudd_zddDumpDot takes an array of nodes.
		DdNode* nodes[1] = {node};
		Cudd_zddDumpDot(global_mgr, 1, nodes, NULL, NULL, fp);
		fclose(fp);
	}

	std::string to_string() {
		std::set<int> v = get_vars();
		std::stringstream s;
		s << "{ ";
		for (auto i : v) {
			s << i << " ";
		}
		s << "}";
		return s.str();
	}

	DdNode* get_node() const {
		return node;
	}
};

class ZDDSLIMOperandIterator {
	ZDD::iterator it;
	ZDD::iterator it_end;

public:
	using iterator_category = std::input_iterator_tag;
	using value_type = SLIMOperand*;
	using difference_type = std::ptrdiff_t;
	using pointer = SLIMOperand*;
	using reference = SLIMOperand*;

	ZDDSLIMOperandIterator(const ZDD& zdd)
		: it(zdd.begin()), it_end(zdd.end()) {}

	// For end iterator
	ZDDSLIMOperandIterator(ZDD::iterator end_it)
		: it(end_it), it_end(end_it) {}

	reference operator*() const {
		return id_mapper.get_object_from_id(*it);
	}

	pointer operator->() const {
		return id_mapper.get_object_from_id(*it);
	}

	ZDDSLIMOperandIterator& operator++() {
		++it;
		return *this;
	}

	bool operator==(const ZDDSLIMOperandIterator& other) const {
		return it == other.it;
	}

	bool operator!=(const ZDDSLIMOperandIterator& other) const {
		return !(*this == other);
	}

	ZDDSLIMOperandIterator begin() const { return *this; }
	ZDDSLIMOperandIterator end() const { return ZDDSLIMOperandIterator(it_end); }
};


#endif