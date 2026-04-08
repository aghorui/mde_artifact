#ifndef LFCPA_COMMON_H
#define LFCPA_COMMON_H
// #include "Analysis.h"
#include "CommonAll.h"
#include "slim/IR.h"

#define MDE_ENABLE_DEBUG
#define MDE_ENABLE_PERFORMANCE_METRICS
#include "mde/mde.hpp"

#include <cstddef>
#include <llvm-14/llvm/Support/raw_ostream.h>

// #define ENABLE_INTEGRITY_CHECKING

#define GET_KEY(__x) ((__x).get_key())
#define GET_VALUE(__x) (std::get<0>((__x).get_value()))

#define GET_PTSET_KEY(__x) ((__x).first)
#define GET_PTSET_VALUE(__x) ((__x).second)

struct SLIMOperandConfig: mde::MDEConfig<SLIMOperand *> {
    using PropertyLess    = SLIMOperandLess;
    using PropertyHash    = SLIMOperandHash;
    using PropertyEqual   = SLIMOperandEqual;
    using PropertyPrinter = SLIMOperandPrinter;
};

struct LivenessMDE
    : public mde::MDENode<SLIMOperandConfig> {

    Index get_purely_global(Index a) {
        STAT_operation_count++;
        if (is_empty(a)) {
            return a;
        }

        PropertySet new_set;

        for (const PropertyElement &i : get_value(a)) {
            if (i.get_key()->isVariableGlobal()) {
                MDE_PUSH_ONE(new_set, i);
            }
        }

        return register_set(new_set);
    }

    Index get_purely_local(Index a) {
        STAT_operation_count++;
        if (is_empty(a)) {
            return a;
        }

        PropertySet new_set;

        for (const PropertyElement &i : get_value(a)) {
            if (!i.get_key()->isVariableGlobal()) {
                MDE_PUSH_ONE(new_set, i);
            }
        }

        return register_set(new_set);
    }
};

extern LivenessMDE livenessMDE;

struct LFLivenessSet {
    using key_type = SLIMOperand *;
    using value_type = SLIMOperand *;
    using iterator = typename LivenessMDE::PropertySet::iterator;
    using const_iterator = typename LivenessMDE::PropertySet::const_iterator;
    using size_type = typename LivenessMDE::PropertySet::size_type;

    using Index = LivenessMDE::Index;
    using PropertyElement = LivenessMDE::PropertyElement;

    Index set_value = mde::EMPTY_SET_VALUE;

    LFLivenessSet() {}

    ~LFLivenessSet() {}

    LFLivenessSet(const Index &b) {
#ifdef ENABLE_INTEGRITY_CHECKING
        // std::cout << "liveness set_value: " << set_value << "\n";
        assert(set_value.value >= 0 && set_value < livenessMDE.property_set_count());
        // CHECK SET INTEGRITY
        std::set<SLIMOperand *> k;
        for (const PropertyElement &i : livenessMDE.get_value(b)) {
            assert(k.count(i.get_key()) == 0);
            k.insert(i.get_key());
        }
#endif
        set_value = b;
    }

    LFLivenessSet(const Index &&b) {
        // std::cout << "liveness set_value: " << set_value << "\n";
#ifdef ENABLE_INTEGRITY_CHECKING
        assert(set_value.value >= 0 && set_value.value < livenessMDE.property_set_count());
#endif
        set_value = b;
    }

    static LFLivenessSet create_from_single(SLIMOperand *p) {
        return livenessMDE.register_set_single(p);
    }

    bool operator<(const LFLivenessSet &b) const {
        STAT_operation_count++;
        return set_value < b.set_value;
    }

    bool operator==(const LFLivenessSet &b) const {
        STAT_operation_count++;
        return set_value == b.set_value;
    }

    LFLivenessSet &operator=(const Index &b) {
        set_value = b;
        return *this;
    }

    LFLivenessSet &operator=(const Index &&b) {
        set_value = b;
        return *this;
    }

    LFLivenessSet set_union(const LFLivenessSet &b) const {
        STAT_operation_count++;
        return livenessMDE.set_union(set_value, b.set_value);
    }

    LFLivenessSet set_intersection(const LFLivenessSet &b) const {
        STAT_operation_count++;
        return livenessMDE.set_intersection(set_value, b.set_value);
    }

    LFLivenessSet get_purely_global() const {
        STAT_operation_count++;
        return LFLivenessSet(livenessMDE.get_purely_global(set_value));
    }
    LFLivenessSet get_purely_local() const {
        STAT_operation_count++;
        return LFLivenessSet(livenessMDE.get_purely_local(set_value));
    }

    LFLivenessSet insert_single(SLIMOperand *p) const {
        STAT_operation_count++;
        return livenessMDE.set_insert_single(set_value, p);
    }

    LFLivenessSet remove_single(SLIMOperand *p) const {
        STAT_operation_count++;
        return livenessMDE.set_remove_single(set_value, p);
    }

    const LivenessMDE::PropertySet &get_value() const {
        return livenessMDE.get_value(set_value);
    }

    bool contains(SLIMOperand *p) const {
        STAT_operation_count++;
        return livenessMDE.contains(set_value, p);
    }

    const_iterator begin() const {
        return livenessMDE.get_value(set_value).begin();
    }

    const_iterator end() const {
        return livenessMDE.get_value(set_value).end();
    }

    bool empty() const {
        return set_value == mde::EMPTY_SET_VALUE;
    }

    std::size_t size() const {
        return livenessMDE.size_of(set_value);
    }
};

struct LFPointsToSet {
    using container_type = std::map<SLIMOperand *, LFLivenessSet>;
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
                (cursor_1->second == cursor_2->second)) {
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
                    new_set.insert({
                        cursor_1->first,
                        cursor_1->second.set_union(cursor_2->second)});
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
        elems[pointer] = elems[pointer].insert_single(pointee);
        return std::move(elems);
    }

    LFPointsToSet update_pointees(SLIMOperand *pointer, LFLivenessSet pointees) {
        STAT_operation_count++;
        elems.insert_or_assign(pointer, pointees);
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
