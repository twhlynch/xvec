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
	vec::vector vector_int {1, 2, 3};
	CustomVector custom_int {1, 2, 3};
	vec::vector vector_float {1.0f, 2.0f, 3.0f};
	CustomVector custom_float {1.0f, 2.0f, 3.0f};
	vec::vector vector_complex {1, 2.0f, 3.0};

	is_same<vec::vector<int, int, int>>(vector_int + custom_int);
	is_same<vec::vector<float, float, float>>(vector_float + custom_float);
	is_same<vec::vector<int, float, double>>(vector_complex + custom_int);
	is_same<vec::vector<int, int, int>>(vector_int + custom_int);
	is_same<CustomVector<int>>(custom_int + vector_int);
	is_same<CustomVector<int>>(custom_int + vector_float);
	is_same<CustomVector<int>>(custom_int + vector_complex);
	is_same<vec::vector<float, float, float>>(vector_int + vector_float);
	is_same<vec::vector<int, int, int>>(vector_int + vector_int);
	is_same<vec::vector<int, float, double>>(vector_int + vector_complex);

	is_same<vec::vector<int, int, int>>(vector_int += custom_int);
	is_same<vec::vector<float, float, float>>(vector_float += custom_float);
	is_same<vec::vector<int, float, double>>(vector_complex += custom_int);
	is_same<vec::vector<int, int, int>>(vector_int += custom_int);
	is_same<CustomVector<int>>(custom_int += vector_int);
	is_same<CustomVector<int>>(custom_int += vector_float);
	is_same<CustomVector<int>>(custom_int += vector_complex);
	is_same<vec::vector<float, float, float>>(vector_int += vector_float);
	is_same<vec::vector<int, int, int>>(vector_int += vector_int);
	is_same<vec::vector<int, float, double>>(vector_int += vector_complex);

	is_same<vec::vector<int, int, int>>(vec::vector(0, 0, 0));
	is_same<vec::vector<double, double, double>>(vec::vector(0.0, 0.0, 0.0));
	is_same<vec::vector<float, float, float>>(vec::vector(0.0f, 0.0f, 0.0f));
	is_same<vec::vector<int, double, float>>(vec::vector(0, 0.0, 0.0f));

	is_same<vec::vector<int, int, int>>(vec::vector {vector_int});
	is_same<vec::vector<float, float, float>>(vec::vector {vector_float});
	is_same<vec::vector<int, int, int>>(vec::vector {custom_int});
	is_same<vec::vector<float, float, float>>(vec::vector {custom_float});
	is_same<vec::vector<int, int, int>>(vec::vector {std::move(custom_int)});
	is_same<vec::vector<float, float, float>>(vec::vector {std::move(custom_float)});

	vec::vector _1 = vector_int;
	is_same<vec::vector<int, int, int>>(_1);
	vec::vector _2 = vector_float;
	is_same<vec::vector<float, float, float>>(_2);
	vec::vector _3 = custom_int;
	is_same<vec::vector<int, int, int>>(_3);
	vec::vector _4 = custom_float;
	is_same<vec::vector<float, float, float>>(_4);
	vec::vector _5 = std::move(custom_int);
	is_same<vec::vector<int, int, int>>(_5);
	vec::vector _6 = std::move(custom_float);
	is_same<vec::vector<float, float, float>>(_6);

	is_same<vec::vector<int, int, int>>(vector_int = vector_float);
	is_same<vec::vector<int, int, int>>(vector_int = vector_complex);
	is_same<CustomVector<int>>(custom_int = vector_int);

	return EXIT_SUCCESS;
}
