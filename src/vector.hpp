#include <concepts>

namespace vec {

template <typename T>
concept Number =
	std::integral<std::remove_cvref_t<T>> || std::floating_point<std::remove_cvref_t<T>>;

template <typename T>
concept VectorLike = requires(T vec) {
	{ vec.x } -> Number;
	{ vec.y } -> Number;
	{ vec.z } -> Number;
};

template <Number X, Number Y, Number Z>
struct vector
{
	X x;
	Y y;
	Z z;

	vector() = default;
	vector(X x, Y y, Z z) : x(x), y(y), z(z) {};
	template <Number T>
	vector(T n) : x(n), y(n), z(n)
	{
	}

	template <VectorLike T>
	vector(const T &v) : x(v.x), y(v.y), z(v.z) {};
	template <VectorLike T>
	vector(T &&v) : x(v.x), y(v.y), z(v.z) {};

	template <VectorLike T>
	vector &operator=(const T &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	};
	template <VectorLike T>
	vector &operator=(T &&v)
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	};

	template <VectorLike T>
	friend vector operator+(vector lhs, const T &rhs)
	{
		return {
			lhs.x + rhs.x,
			lhs.y + rhs.y,
			lhs.z + rhs.z,
		};
	}
	template <VectorLike T>
		requires(!std::same_as<std::remove_cvref_t<T>, vector>)
	friend T operator+(T lhs, const vector &rhs)
	{
		return {
			lhs.x + rhs.x,
			lhs.y + rhs.y,
			lhs.z + rhs.z,
		};
	}

	template <VectorLike T>
		requires(!std::same_as<std::remove_cvref_t<T>, vector>)
	vector operator+=(const T &v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}
};

template <VectorLike T, Number X, Number Y, Number Z>
T operator+=(T &lhs, const vector<X, Y, Z> &rhs)
{
	lhs->x += rhs.x;
	lhs->y += rhs.y;
	lhs->z += rhs.z;

	return lhs;
}

} // namespace vec
