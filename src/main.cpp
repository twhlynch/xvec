#include <cstdlib>
#include <utility>

#include "vector.hpp"

template <typename T>
struct CustomVector
{
	T x;
	T y;
	T z;
};

template <typename Expected, typename Actual>
	requires(std::is_same_v<std::remove_cvref_t<Actual>, Expected>)
static void is_same(Actual &&_)
{
}

int main()
{
	xvec::vector vector_int {1, 2, 3};
	CustomVector custom_int {.x = 1, .y = 2, .z = 3};
	xvec::vector vector_float {1.0f, 2.0f, 3.0f};
	CustomVector custom_float {.x = 1.0f, .y = 2.0f, .z = 3.0f};
	xvec::vector vector_complex {1, 2.0f, 3.0};

	is_same<xvec::vector<int, int, int>>(vector_int + custom_int);
	is_same<xvec::vector<float, float, float>>(vector_float + custom_float);
	is_same<xvec::vector<int, float, double>>(vector_complex + custom_int);
	is_same<xvec::vector<int, int, int>>(vector_int + custom_int);
	is_same<CustomVector<int>>(custom_int + vector_int);
	is_same<CustomVector<int>>(custom_int + vector_float);
	is_same<CustomVector<int>>(custom_int + vector_complex);
	is_same<xvec::vector<float, float, float>>(vector_int + vector_float);
	is_same<xvec::vector<int, int, int>>(vector_int + vector_int);
	is_same<xvec::vector<int, float, double>>(vector_int + vector_complex);

	is_same<xvec::vector<int, int, int>>(vector_int += custom_int);
	is_same<xvec::vector<float, float, float>>(vector_float += custom_float);
	is_same<xvec::vector<int, float, double>>(vector_complex += custom_int);
	is_same<xvec::vector<int, int, int>>(vector_int += custom_int);
	is_same<CustomVector<int>>(custom_int += vector_int);
	is_same<CustomVector<int>>(custom_int += vector_float);
	is_same<CustomVector<int>>(custom_int += vector_complex);
	is_same<xvec::vector<float, float, float>>(vector_int += vector_float);
	is_same<xvec::vector<int, int, int>>(vector_int += vector_int);
	is_same<xvec::vector<int, float, double>>(vector_int += vector_complex);

	is_same<xvec::vector<int, int, int>>(xvec::vector(0, 0, 0));
	is_same<xvec::vector<double, double, double>>(xvec::vector(0.0, 0.0, 0.0));
	is_same<xvec::vector<float, float, float>>(xvec::vector(0.0f, 0.0f, 0.0f));
	is_same<xvec::vector<int, double, float>>(xvec::vector(0, 0.0, 0.0f));

	is_same<xvec::vector<int, int, int>>(xvec::vector {vector_int});
	is_same<xvec::vector<float, float, float>>(xvec::vector {vector_float});
	is_same<xvec::vector<int, int, int>>(xvec::vector {custom_int});
	is_same<xvec::vector<float, float, float>>(xvec::vector {custom_float});
	is_same<xvec::vector<int, int, int>>(xvec::vector {std::move(custom_int)});			// NOLINT(performance-move-const-arg)
	is_same<xvec::vector<float, float, float>>(xvec::vector {std::move(custom_float)}); // NOLINT(performance-move-const-arg)

	xvec::vector _1 = vector_int;
	is_same<xvec::vector<int, int, int>>(_1);
	xvec::vector _2 = vector_float;
	is_same<xvec::vector<float, float, float>>(_2);
	xvec::vector _3 = custom_int;
	is_same<xvec::vector<int, int, int>>(_3);
	xvec::vector _4 = custom_float;
	is_same<xvec::vector<float, float, float>>(_4);
	xvec::vector _5 = std::move(custom_int); // NOLINT(performance-move-const-arg)
	is_same<xvec::vector<int, int, int>>(_5);
	xvec::vector _6 = std::move(custom_float); // NOLINT(performance-move-const-arg)
	is_same<xvec::vector<float, float, float>>(_6);

	is_same<xvec::vector<int, int, int>>(vector_int = vector_float);
	is_same<xvec::vector<int, int, int>>(vector_int = vector_complex);
	is_same<CustomVector<int>>(custom_int = vector_int);

	constexpr xvec::vector cv1 {1, 2, 3};
	constexpr xvec::vector cv2 {4, 5, 6};
	constexpr CustomVector cc1 {.x = 1, .y = 2, .z = 3};
	[[maybe_unused]] constexpr auto cv3 = cv1 + cv2;
	[[maybe_unused]] constexpr auto cv4 = cv1 + cc1;
	[[maybe_unused]] constexpr auto cv5 = static_cast<CustomVector<int>>(cv1);
	[[maybe_unused]] constexpr auto cv6 = cc1 + cv1;

	return EXIT_SUCCESS;
}
