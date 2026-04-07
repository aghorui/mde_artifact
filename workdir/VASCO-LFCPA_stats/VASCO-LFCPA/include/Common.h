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

#define GET_PTSET_KEY(__x) ((__x).get_key())
#define GET_PTSET_VALUE(__x) (std::get<0>((__x).get_value()))

struct SLIMOperandConfig: mde::MDEConfig<SLIMOperand *> {
    using PropertyLess    = SLIMOperandLess;
    using PropertyHash    = SLIMOperandHash;
    using PropertyEqual   = SLIMOperandEqual;
    using PropertyPrinter = SLIMOperandPrinter;
};

extern size_t STAT_operation_count;
extern size_t STAT_equality_comparisons;
extern size_t STAT_enumerations_liveness;
extern size_t STAT_enumerations_pointsto;
extern size_t STAT_assignments_liveness;
extern size_t STAT_assignments_pointsto;
extern size_t STAT_constructions_liveness;
extern size_t STAT_constructions_pointsto;

struct LivenessMDE
    : public mde::MDENode<SLIMOperandConfig> {

    Index get_purely_global(Index a) {
        //STAT_operation_count++;
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
        //STAT_operation_count++;
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

struct PointsToMDE :
    public mde::MDENode<
        SLIMOperandConfig,
        mde::NestingBase<SLIMOperand *, LivenessMDE>> {

    PointsToMDE(LivenessMDE &l) : MDENode(RefList{l}) {}

    // TODO REIMPLEMENT THIS
    Index update_pointees(Index set_value, PropertyElement k) {
        const PropertySet &first = get_value(set_value);
        PropertySet new_set;
        bool inserted = false;
        auto cursor_1 = first.begin();
        const auto &cursor_end_1 = first.end();

        while (cursor_1 != cursor_end_1) {
            if (less_key(k, *cursor_1)) {
                MDE_PUSH_ONE(new_set, k);
                MDE_PUSH_RANGE(new_set, cursor_1, cursor_end_1);
                inserted = true;
                break;
            } else {
                if (!(less_key(*cursor_1, k))) {
                    MDE_PUSH_ONE(new_set, k);
                    cursor_1++;
                    MDE_PUSH_RANGE(new_set, cursor_1, cursor_end_1);
                    inserted = true;
                    break;
                } else {
                    MDE_PUSH_ONE(new_set, *cursor_1);
                }
                cursor_1++;
            }
        }

        if (!inserted) {
            MDE_PUSH_ONE(new_set, k);
        }

        return register_set(std::move(new_set));
    }

    Index insert_pointee(Index set_value, PropertyElement k) {
        Index insertee = register_set_single(k);
        return set_union(set_value, insertee);
    }

    // This also needs to handle the empty set condition, apparently.
    LivenessMDE::Index get_pointees(Index set_value, SLIMOperand *pointer) {
        const PropertySet &first = get_value(set_value);
        for (auto &elem : first) {
            // POTENTIAL PROBLEM?
            if (GET_KEY(elem) == pointer) {
                return GET_VALUE(elem);
            }
        }
        return mde::EMPTY_SET_VALUE;
    }
};

extern LivenessMDE livenessMDE;
extern PointsToMDE pointsToMDE;

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
        STAT_constructions_liveness++;
        set_value = b;
    }

    LFLivenessSet(const Index &&b) {
        // std::cout << "liveness set_value: " << set_value << "\n";
#ifdef ENABLE_INTEGRITY_CHECKING
        assert(set_value.value >= 0 && set_value.value < livenessMDE.property_set_count());
#endif
        STAT_constructions_liveness++;
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
        STAT_equality_comparisons++;
        return set_value == b.set_value;
    }

    LFLivenessSet &operator=(const Index &b) {
        STAT_assignments_liveness++;
        set_value = b;
        return *this;
    }

    LFLivenessSet &operator=(const Index &&b) {
        STAT_assignments_liveness++;
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
        STAT_enumerations_pointsto++;
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
    using key_type = SLIMOperand *;
    using value_type = SLIMOperand *;
    using iterator = typename PointsToMDE::PropertySet::iterator;
    using const_iterator = typename PointsToMDE::PropertySet::const_iterator;
    using size_type = typename PointsToMDE::PropertySet::size_type;

    using Index = PointsToMDE::Index;
    using PropertyElement = PointsToMDE::PropertyElement;

    Index set_value = mde::EMPTY_SET_VALUE;

    LFPointsToSet() {
        // nop
    }

    LFPointsToSet(Index set_value) : set_value(set_value) {
        // std::cout << "ptset set_value: " << set_value << "\n";
        // std::cout << "operand1: { ";
        // for (auto i : pointsToMDE.get_value(set_value)) {
        //     std::cout << "(" << i.key << ", " << i.value << "),";
        // }
        // std::cout << " }" << std::endl;
#ifdef ENABLE_INTEGRITY_CHECKING
        assert(set_value.value >= 0 && set_value < pointsToMDE.property_set_count());
        // CHECK SET INTEGRITY
        std::set<SLIMOperand *> k;
        for (const PropertyElement &i : pointsToMDE.get_value(set_value)) {
            // std::cout << "checking: (" << i.key << ", " << i.value << ")" <<
            // std::endl;
            assert(k.count(GET_KEY(i)) == 0);
            k.insert(GET_KEY(i));
        }
#endif
        STAT_constructions_pointsto++;
    }

    ~LFPointsToSet() {}

    bool operator<(const LFPointsToSet &b) const {
        STAT_operation_count++;
        return set_value < b.set_value;
    }

    bool operator==(const LFPointsToSet &b) const {
        STAT_operation_count++;
        STAT_equality_comparisons++;
        return set_value == b.set_value;
    }

    bool empty() const {
        return set_value == mde::EMPTY_SET_VALUE;
    }

    LFPointsToSet set_union(const LFPointsToSet &b) const {
        STAT_operation_count++;
        return pointsToMDE.set_union(set_value, b.set_value);
    }

    LFPointsToSet insert_pointee(SLIMOperand *pointer, SLIMOperand *pointee) {
        STAT_operation_count++;
        LivenessMDE::Index pointeeSet = livenessMDE.register_set_single(pointee);
        return pointsToMDE.insert_pointee(set_value, {pointer, pointeeSet});
    }

    LFPointsToSet update_pointees(SLIMOperand *pointer, LFLivenessSet pointees) {
        STAT_operation_count++;
        return pointsToMDE.update_pointees(set_value, {pointer, pointees.set_value});
    }

    LFLivenessSet get_pointees(SLIMOperand *pointer) {
        return pointsToMDE.get_pointees(set_value, pointer);
    }

    const PointsToMDE::PropertySet &get_value() const {
        return pointsToMDE.get_value(set_value);
    }

    std::size_t size() const {
        return pointsToMDE.size_of(set_value);
    }

    const_iterator begin() const {
        STAT_enumerations_liveness++;
        return pointsToMDE.get_value(set_value).begin();
    }

    const_iterator end() const {
        return pointsToMDE.get_value(set_value).end();
    }
};

#endif
