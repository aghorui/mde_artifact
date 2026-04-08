#include "mde/mde.hpp"
#include <gtest/gtest.h>
#include <utility>

using StringMDE = mde::MDENode<mde::MDEConfig<std::string>>;
using FloatMDE = mde::MDENode<mde::MDEConfig<float>>;

using TwoNestedMDE = mde::MDENode<
	mde::MDEConfig<int>,
	mde::NestingBase<int, StringMDE, FloatMDE>>;

TEST(MDE_NestingChecks, check_property_element_api) {
	StringMDE s;
	FloatMDE f;
	TwoNestedMDE l({s, f});

	std::string stra = "sad";
	std::string strb = "zxc";
	StringMDE::Index sa = s.register_set({ std::move(stra), std::move(strb) });
	FloatMDE::Index fa = f.register_set({123.23, 4213.324});

	TwoNestedMDE::Index ta = l.register_set({
		{1, {sa, fa}}
	});

	for (TwoNestedMDE::PropertyElement i : l.get_value(ta)) {
		std::cout << i.value0().value << std::endl;
		std::cout << i.value<1>().value << std::endl;
	}

	std::cout << s.dump() << std::endl;
	std::cout << f.dump() << std::endl;
	std::cout << l.dump() << std::endl;
}