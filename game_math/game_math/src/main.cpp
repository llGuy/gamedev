#include "vector.h"

auto main(void) -> int
{
	using namespace gml;

	vector<float, 4> a(1.0f, 0.0f, 0.0f, 0.0f);
	vector<float, 4> b(0.0f, 0.0f, 0.0f, 0.0f);

	auto d = dot(a, b);
}