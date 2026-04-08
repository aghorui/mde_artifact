#include "common.hpp"
#include "mde/mde.hpp"
#include <cmath>
#include <sstream>

#ifdef MDE_ENABLE_SERIALIZATION

using MDE = MDEVerify<mde::MDEConfig<int>>;
using Index = typename MDE::Index;

TEST(MDE_SerializationChecks, check_serialization) {
	MDE l;
	Index a = l.register_set({1, 2, 3});
	Index b = l.register_set({4, 5, 6});
	l.set_union(a, b);
	std::cout << l.dump() << std::endl;
	auto j = l.to_json();
	MDE l2;
	l2.load_from_json(j);
	std::cout << l2.dump() << std::endl;
}

using PointeeMDE = mde::MDENode<mde::MDEConfig<int>>;
using PointerMDE = mde::MDENode<
	mde::MDEConfig<int>,
	mde::NestingBase<int, PointeeMDE, PointeeMDE>
>;

TEST(MDE_SerializationChecks, check_mde_walk) {
	PointeeMDE p;
	PointerMDE l({p, p});
	auto a = p.register_set({1, 2, 3});
	auto b = p.register_set({4, 5, 6});
	p.set_union(a, b);

	l.register_set({{2, {a, b}}});

	mde::slz::JSON data = mde::slz::mde_to_json(l);
	std::cout << data << std::endl;

	PointeeMDE p2;
	PointerMDE l2({p2, p2});
	mde::slz::mde_from_json(l2, data);

	std::cout << l2.dump() << std::endl;

	ASSERT_EQ(l.dump(), l2.dump());
}

#endif