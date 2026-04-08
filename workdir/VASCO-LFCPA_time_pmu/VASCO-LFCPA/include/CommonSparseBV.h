#ifndef LFCPA_COMMON_SparseBV_H
#define LFCPA_COMMON_SparseBV_H
// #include "Analysis.h"
#include "slim/IR.h"
#include <utility>
#include "SparseBVWrapper.h"
#include "CommonAll.h"

#include <cstddef>
#include <llvm-14/llvm/Support/raw_ostream.h>

#define GET_KEY(__x) ((__x))
#define GET_VALUE(__x) ((__x).second)

#define GET_PTSET_KEY(__x) ((__x).first)
#define GET_PTSET_VALUE(__x) ((__x).second)

template<typename SetT>
static inline bool setLessThan(const SetT &a, const SetT &b) {
    if (a.size() > b.size()) {
        return true;
    }

    if (a.size() == 0) {
        return false;
    }

    auto cursor_1 = a.begin();
    const auto &cursor_end_1 = a.end();
    auto cursor_2 = b.begin();
    const auto &cursor_end_2 = b.end();

    while (cursor_1 != cursor_end_1) {
        if (SLIMOperandLess()(*cursor_1, *cursor_2)) {
            cursor_1++;
            cursor_2++;
        } else {
            return true;
        }
    }

    return false;
}

template<typename SetT>
static inline bool setEqual(const SetT &a, const SetT &b) {
    if (a.size() != b.size()) {
        return false;
    }

    if (a.size() == 0) {
        return true;
    }

    auto cursor_1 =a.begin();
    const auto &cursor_end_1 = a.end();
    auto cursor_2 = b.begin();
    const auto &cursor_end_2 = b.end();

    while (cursor_1 != cursor_end_1) {
        if (SLIMOperandEqual()(*cursor_1, *cursor_2)) {
            cursor_1++;
            cursor_2++;
        } else {
            return false;
        }
    }

    return true;
}

struct LFLivenessSet {
    using container_type = SparseBV;
    using key_type = SLIMOperand *;
    using value_type = SLIMOperand *;
    using iterator = SparseBVSLIMOperandIterator;
    using size_type = std::size_t;

    SparseBV elems;

    LFLivenessSet() {}

    ~LFLivenessSet() {}

    LFLivenessSet(container_type &elems):
        elems(elems) {}

    LFLivenessSet(container_type &&elems):
        elems(elems) {}

    static LFLivenessSet create_from_single(key_type p) {
        STAT_operation_count++;
        return LFLivenessSet(SparseBV(id_mapper.get_id(p)));
    }

    bool operator<(const LFLivenessSet &b) const {
        STAT_operation_count++;
        return elems < b.elems;
    }

    bool operator==(const LFLivenessSet &b) const {
        return elems  == b.elems;
    }

    LFLivenessSet &operator=(const container_type &b) {
        elems = b;
        return *this;
    }

    LFLivenessSet &operator=(const container_type &&b) {
        elems = b;
        return *this;
    }

    LFLivenessSet set_union(const LFLivenessSet &b) const {
        STAT_operation_count++;
        return LFLivenessSet(elems.set_union(b.elems));
    }

    LFLivenessSet set_intersection(const LFLivenessSet &b) const {
        STAT_operation_count++;
        return LFLivenessSet(elems.set_intersection(b.elems));
    }

    LFLivenessSet get_purely_global() const {
        STAT_operation_count++;
        if (empty()) {
            return LFLivenessSet();
        }

        container_type new_set;

        for (SLIMOperand *i : SparseBVSLIMOperandIterator(elems)) {
            if (i->isVariableGlobal()) {
                new_set = new_set.insert_single(id_mapper.get_id(i));
            }
        }

        return std::move(new_set);
    }

    LFLivenessSet get_purely_local() const {
        STAT_operation_count++;
        if (empty()) {
            return LFLivenessSet();
        }

        container_type new_set;

        for (SLIMOperand *i : SparseBVSLIMOperandIterator(elems)) {
            if (!i->isVariableGlobal()) {
                new_set = new_set.insert_single(id_mapper.get_id(i));
            }
        }

        return std::move(new_set);
    }

    LFLivenessSet insert_single(key_type p) {
        STAT_operation_count++;
        SparseBV result = elems.insert_single(id_mapper.get_id(p));
        assert(result.size() > 0);
        return std::move(result);
    }

    LFLivenessSet remove_single(key_type p) {
        STAT_operation_count++;
        SparseBV result = elems.remove_single(id_mapper.get_id(p));
        return std::move(result);
    }

    bool contains(SLIMOperand *p) const {
        STAT_operation_count++;
        return elems.contains(id_mapper.get_id(p)) > 0;
    }

    SparseBVSLIMOperandIterator begin() const {
        return SparseBVSLIMOperandIterator(elems).begin();
    }

    SparseBVSLIMOperandIterator end() const {
        return SparseBVSLIMOperandIterator(elems).end();
    }

    bool empty() const {
        return elems.is_empty();
    }

    std::size_t size() const {
        return elems.size();
    }
};

struct LFPointsToSet {
    using container_type = std::map<SLIMOperand *, SparseBV>;
    using key_type = SLIMOperand *;
    using value_type = std::set<SLIMOperand *>;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    using size_type = typename container_type::size_type;

    container_type elems;

    LFPointsToSet() {}

    ~LFPointsToSet() {}

    LFPointsToSet(container_type &elems):
        elems(elems) {}

    LFPointsToSet(container_type &&elems):
        elems(elems) {}

    bool operator<(const LFPointsToSet &b) const {
        STAT_operation_count++;
        return elems < b.elems;
    }

    bool operator==(const LFPointsToSet &b) const {
        STAT_operation_count++;
        if (elems.size() != b.elems.size()) {
            return false;
        }

        if (elems.size() == 0) {
            return true;
        }

        auto cursor_1 = elems.begin();
        const auto &cursor_end_1 = elems.end();
        auto cursor_2 = b.elems.begin();
        const auto &cursor_end_2 = b.elems.end();

        while (cursor_1 != cursor_end_1) {
            if (compareOperands(cursor_1->first, cursor_2->first) &&
                cursor_1->second == cursor_2->second) {
                cursor_1++;
                cursor_2++;
            } else {
                return false;
            }
        }

        return true;
    }

    bool empty() const {
        return elems.size() == 0;
    }

    LFPointsToSet set_union(const LFPointsToSet &b) const {
        STAT_operation_count++;
        container_type new_set;

        auto cursor_1 = elems.begin();
        const auto &cursor_end_1 = elems.end();
        auto cursor_2 = b.elems.begin();
        const auto &cursor_end_2 = b.elems.end();

        while (cursor_1 != cursor_end_1) {
            if (cursor_2 == cursor_end_2) {
                new_set.insert(cursor_1, cursor_end_1);
                break;
            }

            if (SLIMOperandLess()(cursor_2->first, cursor_1->first)) {
                new_set.insert(*cursor_2);
                cursor_2++;
            } else {
                if (!(SLIMOperandLess()(cursor_1->first, cursor_2->first))) {
                    new_set.insert({cursor_1->first, cursor_1->second.set_union(cursor_2->second)});
                    cursor_2++;
                } else {
                    new_set.insert(*cursor_1);
                }
                cursor_1++;
            }
        }

        new_set.insert(cursor_2, cursor_end_2);

        return std::move(new_set);
    }

    LFPointsToSet insert_pointee(SLIMOperand *pointer, SLIMOperand *pointee) {
        STAT_operation_count++;
        elems[pointer] = elems[pointer].insert_single(id_mapper.get_id(pointee));
        return std::move(elems);
    }

    LFPointsToSet update_pointees(SLIMOperand *pointer, LFLivenessSet pointees) {
        STAT_operation_count++;
        elems.insert_or_assign(pointer, pointees.elems);
        return std::move(elems);
    }

    LFLivenessSet get_pointees(SLIMOperand *pointer) {
        STAT_operation_count++;
        return elems.at(pointer);
    }

    std::size_t size() const {
        return elems.size();
    }

    const_iterator begin() const {
        return elems.begin();
    }

    const_iterator end() const {
        return elems.end();
    }
};

#endif
