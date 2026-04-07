#include "common.hpp"
#include <gtest/gtest.h>

template <typename T>
class MDE_InitCheck : public ::testing::Test {
public:
	using MDE = MDEVerify<mde::MDEConfig<T>>;
	using Index = typename MDE::Index;
	MDE l;
};

TYPED_TEST_SUITE(MDE_InitCheck, DefaultTestingTypes);

TYPED_TEST(MDE_InitCheck, contains_only_one_elem) {
	ASSERT_EQ(this->l.property_set_count(), 1);
}

TYPED_TEST(MDE_InitCheck, sole_elem_is_empty) {
	using Index = typename MDE_InitCheck<TypeParam>::Index;
	ASSERT_EQ(this->l.get_value(Index(0)).size(), 0);
}

TYPED_TEST(MDE_InitCheck, operation_maps_are_empty) {
	ASSERT_TRUE(this->l.verify_relation_map_init_sizes());
}

TYPED_TEST(MDE_InitCheck, subset_is_empty) {
	ASSERT_TRUE(this->l.verify_relation_map_init_sizes());
}