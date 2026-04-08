#ifndef SPARSEBV_WRAPPER_H
#define SPARSEBV_WRAPPER_H
#include "CommonAll.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <math.h>
#include <llvm/ADT/SparseBitVector.h>

class SparseBV {
public:
	using Container = llvm::SparseBitVector<128>;

protected:
	Container sbv;

public:
	SparseBV() {}

	SparseBV(const Container& s) : sbv(s) {}
	SparseBV(const SparseBV& other) : sbv(other.sbv) {}

	template <typename Iterator>
	SparseBV(Iterator begin, Iterator end) {
		for (Iterator it = begin; it != end; ++it) {
			sbv.set(*it);
		}
	}

	SparseBV(std::initializer_list<int> l) {
		for (int i : l) {
			sbv.set(i);
		}
	}

	explicit SparseBV(int i) {
		sbv.set(i);
	}

	bool contains(int index) const {
		return sbv.test(index);
	}

	size_t size() const {
		return sbv.count();
	}

	bool is_empty() const {
		return sbv.empty();
	}

	using iterator = Container::iterator;

	iterator begin() const { return sbv.begin(); }
	iterator end() const { return sbv.end(); }

	SparseBV& operator=(const SparseBV& other) {
		if (this != &other) {
			sbv = other.sbv;
		}
		return *this;
	}

	bool operator==(const SparseBV& other) const {
		return sbv == other.sbv;
	}

	bool operator!=(const SparseBV& other) const {
		return !(*this == other);
	}

	bool operator<(const SparseBV& other) const {
		// Lexicographical comparison for map key behavior
		iterator it1 = begin();
		iterator it2 = other.begin();
		iterator end1 = end();
		iterator end2 = other.end();

		while (it1 != end1 && it2 != end2) {
			if (*it1 < *it2) return true;
			if (*it2 < *it1) return false;
			++it1;
			++it2;
		}
		return (it1 == end1) && (it2 != end2);
	}

	SparseBV set_union(const SparseBV& other) const {
		SparseBV result(*this);
		result.sbv |= other.sbv;
		return result;
	}

	SparseBV set_intersection(const SparseBV& other) const {
		SparseBV result(*this);
		result.sbv &= other.sbv;
		return result;
	}

	SparseBV set_difference(const SparseBV& other) const {
		SparseBV result(*this);
		result.sbv.intersectWithComplement(other.sbv);
		return result;
	}

	SparseBV insert_single(int i) const {
		SparseBV result(*this);
		result.sbv.set(i);
		return result;
	}

	SparseBV remove_single(int i) const {
		SparseBV result(*this);
		result.sbv.reset(i);
		return result;
	}

	std::string to_string() {
		std::stringstream s;
		s << "{ ";
		for (int i : sbv) {
			s << i << " ";
		}
		s << "}";
		return s.str();
	}
};

class SparseBVSLIMOperandIterator {
	SparseBV::iterator it;
	SparseBV::iterator it_end;

public:
	using iterator_category = std::input_iterator_tag;
	using value_type = SLIMOperand*;
	using difference_type = std::ptrdiff_t;
	using pointer = SLIMOperand*;
	using reference = SLIMOperand*;

	SparseBVSLIMOperandIterator(const SparseBV& sbv)
		: it(sbv.begin()), it_end(sbv.end()) {}

	SparseBVSLIMOperandIterator(SparseBV::iterator end_it)
		: it(end_it), it_end(end_it) {}

	reference operator*() const {
		return id_mapper.get_object_from_id(*it);
	}

	pointer operator->() const {
		return id_mapper.get_object_from_id(*it);
	}

	SparseBVSLIMOperandIterator& operator++() {
		++it;
		return *this;
	}

	bool operator==(const SparseBVSLIMOperandIterator& other) const {
		return it == other.it;
	}

	bool operator!=(const SparseBVSLIMOperandIterator& other) const {
		return !(*this == other);
	}

	SparseBVSLIMOperandIterator begin() const { return *this; }
	SparseBVSLIMOperandIterator end() const { return SparseBVSLIMOperandIterator(it_end); }
};

#endif