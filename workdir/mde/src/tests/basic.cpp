#include "common.hpp"
#include "mde/mde.hpp"
#include <gtest/gtest.h>
#include <iostream>

using MDE = MDEVerify<mde::MDEConfig<int>>;
using Index = typename MDE::Index;

TEST(MDE_BasicChecks, empty_set_is_index_0) {
	MDE l;
	std::cout << l.dump() << std::endl;
	// Repetition is intentional
	ASSERT_EQ(l.register_set({}).value, mde::EMPTY_SET_VALUE);
	ASSERT_NE(l.register_set({ 1, 2, 3, 4 }).value, mde::EMPTY_SET_VALUE);
	ASSERT_EQ(l.register_set({}).value, mde::EMPTY_SET_VALUE);
	ASSERT_EQ(l.register_set({}).value, mde::EMPTY_SET_VALUE);
}

TEST(MDE_BasicChecks, index_is_empty_func_check) {
	MDE l;
	ASSERT_TRUE(l.register_set({}).is_empty());
}

TEST(MDE_BasicChecks, nonempty_set_is_not_index_0) {
	MDE l;
	ASSERT_NE(l.register_set({ 1, 2, 3, 4 }).value, mde::EMPTY_SET_VALUE);
	ASSERT_EQ(l.register_set({}).value, mde::EMPTY_SET_VALUE);
	ASSERT_NE(l.register_set({ 1, 2, 3, 4 }).value, mde::EMPTY_SET_VALUE);
}

TEST(MDE_BasicChecks, set_index_is_consistent) {
	MDE l;
	ASSERT_EQ(l.register_set({ 1, 2, 3, 4 }).value, l.register_set({ 1, 2, 3, 4 }).value);
	ASSERT_EQ(l.register_set({ 1, 2, 3, 5 }).value, l.register_set({ 1, 2, 3, 5 }).value);
	ASSERT_NE(l.register_set({ 1, 2, 3, 5 }).value, l.register_set({ 1, 2, 3, 4 }).value);
}

TEST(MDE_BasicChecks, empty_operation_check) {
	MDE l;
	Index a = l.register_set({ });
	Index b = l.register_set({ });

	ASSERT_TRUE(l.set_union(a, b).is_empty());
	ASSERT_TRUE(l.set_intersection(a, b).is_empty());
	ASSERT_TRUE(l.set_difference(a, b).is_empty());
}

TEST(MDE_BasicChecks, set_union_integrity_check_empty) {
	MDE l;
	Index a = l.register_set({ });
	Index b = l.register_set({ 1, 2, 3, 5 });
	Index result = l.set_union(a, b);

	ASSERT_NE(result.value, mde::EMPTY_SET_VALUE);
	ASSERT_EQ(b, result);
	ASSERT_NE(a, result);
	// ASSERT_EQ(l.is_subset(a, result), mde::SubsetRelation::SUBSET);
	// ASSERT_EQ(l.is_subset(b, result), mde::SubsetRelation::SUBSET);
}

TEST(MDE_BasicChecks, set_union_integrity_check) {
	MDE l;
	Index a = l.register_set({ 1, 2, 3, 4 });
	Index b = l.register_set({ 1, 2, 3, 5 });
	Index result = l.set_union(a, b);

	ASSERT_NE(result.value, mde::EMPTY_SET_VALUE);
	ASSERT_NE(a, result);
	ASSERT_NE(b, result);
	ASSERT_EQ(result, l.register_set({ 1, 2, 3, 4, 5 }));
	ASSERT_EQ(result, l.set_union(b, a));
	// ASSERT_EQ(l.is_subset(a, result), mde::SubsetRelation::SUBSET);
	// ASSERT_EQ(l.is_subset(b, result), mde::SubsetRelation::SUBSET);

	Index c = l.register_set({ 1, 2, 3, 4, 5 });
	ASSERT_EQ(c, l.set_union(a, c));
	ASSERT_EQ(c, l.set_union(c, a));
}


TEST(MDE_BasicChecks, set_intersection_integrity_check_empty) {
	MDE l;
	Index a = l.register_set({ });
	Index b = l.register_set({ 1, 2, 3, 5 });
	Index result = l.set_intersection(a, b);

	ASSERT_TRUE(result.is_empty());
	// ASSERT_EQ(l.is_subset(a, result), mde::SubsetRelation::SUPERSET);
	// ASSERT_EQ(l.is_subset(b, result), mde::SubsetRelation::SUPERSET);
}

TEST(MDE_BasicChecks, set_intersection_integrity_check) {
	MDE l;
	Index a = l.register_set({ 1, 2, 3, 4 });
	Index b = l.register_set({ 1, 2, 3, 5 });
	Index result = l.set_intersection(a, b);

	ASSERT_FALSE(result.is_empty());
	ASSERT_NE(a, result);
	ASSERT_NE(b, result);
	ASSERT_EQ(result, l.register_set({ 1, 2, 3 }));
	ASSERT_EQ(result, l.set_intersection(b, a));
	// ASSERT_EQ(l.is_subset(a, result), mde::SubsetRelation::SUBSET);
	// ASSERT_EQ(l.is_subset(b, result), mde::SubsetRelation::SUBSET);

	Index c = l.register_set({ 1, 2, 3 });
	ASSERT_EQ(c, l.set_intersection(a, c));
	ASSERT_EQ(c, l.set_intersection(c, a));
}

TEST(MDE_BasicChecks, set_filter_check) {
	MDE l;


	Index a = l.register_set({ 1, 2, 3, 4, 99, 1002 });
	Index b = l.register_set({ 1, 2, 3, 5 });
	Index c = l.register_set({ 5 });

	MDE::UnaryOperationMap f1map;
	auto f1 = [](const MDE::PropertyElement &p){ return p.get_value() < 5; };

	MDE::UnaryOperationMap f2map;
	auto f2 = [](const MDE::PropertyElement &p){ return p.get_value() > 3; };

	Index d = l.set_filter(
		a,
		f1,
		f1map);
	ASSERT_EQ(l.size_of(d), 4);

	Index e = l.set_filter(
		b,
		f1,
		f1map);
	ASSERT_EQ(l.size_of(e), 3);

	Index e2 = l.set_filter(
		b,
		f2,
		f2map);
	ASSERT_EQ(l.size_of(e2), 1);

	Index f = l.set_filter(
		c,
		f1,
		f1map);
	ASSERT_TRUE(f.is_empty());
}

TEST(MDE_BasicChecks, set_contains_tests) {
	MDE l;
	Index empty = l.register_set({});
	Index one_elem = l.register_set({ 2 });
	Index ten_elem = l.register_set({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

	ASSERT_FALSE(l.contains(empty, 10));
	ASSERT_FALSE(l.contains(empty, -1));
	ASSERT_FALSE(l.contains(empty, -500000));
	ASSERT_FALSE(l.contains(empty, 50000000));

	ASSERT_FALSE(l.contains(one_elem, 10));
	ASSERT_FALSE(l.contains(one_elem, -1));
	ASSERT_FALSE(l.contains(one_elem, -500000));
	ASSERT_FALSE(l.contains(one_elem, 50000000));
	ASSERT_TRUE(l.contains(one_elem, 2));

	ASSERT_FALSE(l.contains(ten_elem, 13));
	ASSERT_FALSE(l.contains(ten_elem, -1));
	ASSERT_FALSE(l.contains(ten_elem, 50000000));
	ASSERT_TRUE(l.contains(ten_elem, 1));
	ASSERT_TRUE(l.contains(ten_elem, 2));
	ASSERT_TRUE(l.contains(ten_elem, 3));
	ASSERT_TRUE(l.contains(ten_elem, 4));
	ASSERT_TRUE(l.contains(ten_elem, 5));
	ASSERT_TRUE(l.contains(ten_elem, 6));
	ASSERT_TRUE(l.contains(ten_elem, 7));
	ASSERT_TRUE(l.contains(ten_elem, 8));
	ASSERT_TRUE(l.contains(ten_elem, 9));
	ASSERT_TRUE(l.contains(ten_elem, 10));
}

#define ___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(__set, __value) \
{ \
		auto result = l.find_key((__set), (__value)); \
		ASSERT_TRUE(result.is_present()); \
		ASSERT_EQ(result.get().get_key(), (__value)); \
}

TEST(MDE_BasicChecks, set_find_key_tests) {
	MDE l;
	Index empty = l.register_set({});
	Index one_elem = l.register_set({ 2 });
	Index ten_elem = l.register_set({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

	// Section 1
	ASSERT_FALSE(l.find_key(empty, 10).is_present());
	ASSERT_FALSE(l.find_key(empty, -1).is_present());
	ASSERT_FALSE(l.find_key(empty, -500000).is_present());
	ASSERT_FALSE(l.find_key(empty, 50000000).is_present());

	// Section 2
	ASSERT_FALSE(l.find_key(one_elem, 10).is_present());
	ASSERT_FALSE(l.find_key(one_elem, -1).is_present());
	ASSERT_FALSE(l.find_key(one_elem, -500000).is_present());
	ASSERT_FALSE(l.find_key(one_elem, 50000000).is_present());

	{
		const auto result = l.find_key(one_elem, 2);
		ASSERT_TRUE(result.is_present());
	}

	// Section 3
	ASSERT_FALSE(l.find_key(ten_elem, 13).is_present());
	ASSERT_FALSE(l.find_key(ten_elem, -1).is_present());
	ASSERT_FALSE(l.find_key(ten_elem, 50000000).is_present());

	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 1);
	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 2);
	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 3);
	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 4);
	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 5);
	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 6);
	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 7);
	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 8);
	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 9);
	___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(ten_elem, 10);
}

TEST(MDE_BasicChecks, set_find_key_bsearch_tests) {
	MDE l;

	// Create a vector with elements greater than threshold
	mde::Vector<MDE::PropertyElement> acc;
	for (int i = 0; i < MDE_SORTED_VECTOR_BINARY_SEARCH_THRESHOLD; i++) {
		acc.push_back(i);
	}

	for (int i = 0; i < 12; i++) {
		acc.push_back(MDE_SORTED_VECTOR_BINARY_SEARCH_THRESHOLD + 1000 + i);
	}

	Index s = l.register_set(acc);

	ASSERT_FALSE(l.find_key(s, -1).is_present());
	ASSERT_FALSE(l.find_key(s, 50000000).is_present());

	for (size_t i = 0; i < acc.size(); i++) {
		___MDE_BASICTESTS_FIND_KEY_TEST_EXISTENCE_EQUALITY(s, acc[i].get_key());
	}
}

TEST(MDE_BasicChecks, set_insert_single_integrity) {
	MDE l;

	Index a = l.register_set({});
	ASSERT_EQ(a.value, mde::EMPTY_SET_VALUE);
	a = l.set_insert_single(a, 1);
	ASSERT_EQ(a, l.register_set({ 1 }));
	a = l.set_insert_single(a, 9);
	ASSERT_EQ(a, l.register_set({ 1, 9 }));
	a = l.set_insert_single(a, 21313222);
	ASSERT_EQ(a, l.register_set({ 1, 9, 21313222 }));
	a = l.set_insert_single(a, -123);
	ASSERT_EQ(a, l.register_set({ -123, 1, 9, 21313222 }));
	a = l.set_insert_single(a, 4);
	ASSERT_EQ(a, l.register_set({ -123, 1, 4, 9, 21313222 }));
	a = l.set_insert_single(a, 3);
	ASSERT_EQ(a, l.register_set({ -123, 1, 3, 4, 9, 21313222 }));
}

TEST(MDE_BasicChecks, set_remove_single_integrity) {
	MDE l;

	Index a = l.register_set({ -123, 1, 3, 4, 9, 21313222 });
	a = l.set_remove_single(a, 1);
	ASSERT_EQ(a, l.register_set({ -123, 3, 4, 9, 21313222 }));
	a = l.set_remove_single(a, 9);
	ASSERT_EQ(a, l.register_set({ -123, 3, 4, 21313222 }));
	a = l.set_remove_single(a, 21313222);
	ASSERT_EQ(a, l.register_set({ -123, 3, 4 }));
	a = l.set_remove_single(a, -123);
	ASSERT_EQ(a, l.register_set({ 3, 4 }));
	a = l.set_remove_single(a, 4);
	ASSERT_EQ(a, l.register_set({ 3 }));
	a = l.set_remove_single(a, 3);
	ASSERT_EQ(a.value, mde::EMPTY_SET_VALUE);
}


TEST(MDE_BasicChecks, register_set_iter_test) {
	MDE l;
	std::set<int> payload = { 1, 9, 2, 3, 4, 23, 7, 4, 4, 4, 4, 2 };
	Index a = l.register_set(payload.begin(), payload.end());
	Index b = l.register_set({ 1, 2, 3, 4, 7, 9, 23 });
	ASSERT_EQ(a, b);
}

TEST(MDE_BasicChecks, mde_clear_test) {
	MDE l;

	l.register_set({ -123, 1, 3, 4, 9, 21313222 });
	l.register_set({ -123, 3, 4, 9, 21313222 });
	l.register_set({ -123, 3, 4, 21313222 });
	l.register_set({ -123, 3, 4 });
	l.register_set({ 3, 4 });
	l.register_set({ 3 });
	ASSERT_EQ(l.property_set_count(), 6 + 1);
	l.clear_and_initialize();
	ASSERT_EQ(l.property_set_count(), 1);
}


#ifdef MDE_ENABLE_DEBUG
TEST(MDE_BasicChecks, property_set_out_of_bounds_throws_exception) {
	MDE l;
	l.register_set({ 1, 2, 3, 4 });
	l.register_set_single(2);
	l.register_set_single(3);
	l.register_set_single(4);
	l.register_set_single(5);
	ASSERT_THROW(l.get_value(Index(99999999)), mde::AssertError);
}
#endif