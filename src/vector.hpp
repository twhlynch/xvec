#include <concepts>
#include <type_traits>

namespace vec {

// MARK: concepts

// integral or floating point
// NOTE: includes bool and char types
template <typename T>
concept Number =
	std::integral<std::remove_cvref_t<T>> || std::floating_point<std::remove_cvref_t<T>>;

template <typename T>
concept VectorLike = requires(T vec) {
	// x, y, and z are numbers
	{ vec.x } -> Number;
	{ vec.y } -> Number;
	{ vec.z } -> Number;
};

// MARK: is_vector

// forward declare to use here
template <Number X, Number Y, Number Z>
struct vector;

// is_vector is false for all types except vector
template <typename>
struct is_vector : std::false_type
{
};
template <Number X, Number Y, Number Z>
struct is_vector<vector<X, Y, Z>> : std::true_type
{
};

// a Vector is specifically this vector class
template <typename T>
concept Vector = is_vector<std::remove_cvref_t<T>>::value;

template <typename T>
concept OtherVector = !Vector<T> && VectorLike<T>;

// MARK: class

template <Number X, Number Y, Number Z>
struct vector
{
	X x;
	Y y;
	Z z;

	// constructors
	vector() = default;
	vector(X x, Y y, Z z) : x(x), y(y), z(z) {};
	template <Number T>
	vector(T n) : x(n), y(n), z(n) {};

	template <VectorLike T>
	vector(const T &v) : x(v.x), y(v.y), z(v.z) {};
	template <VectorLike T>
	vector(T &&v) : x(v.x), y(v.y), z(v.z) {};

	// MARK: methods

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

	template <OtherVector T>
	operator T() const
	{ return {x, y, z}; };

	// vec + other
	template <OtherVector T>
	friend vector operator+(vector lhs, const T &rhs)
	{
		return {
			static_cast<X>(lhs.x + rhs.x),
			static_cast<Y>(lhs.y + rhs.y),
			static_cast<Z>(lhs.z + rhs.z),
		};
	}
	// other + vec
	template <OtherVector T>
	friend T operator+(T lhs, const vector &rhs)
	{
		return {
			static_cast<std::remove_cvref_t<decltype(lhs.x)>>(lhs.x + rhs.x),
			static_cast<std::remove_cvref_t<decltype(lhs.y)>>(lhs.y + rhs.y),
			static_cast<std::remove_cvref_t<decltype(lhs.z)>>(lhs.z + rhs.z),
		};
	}
	// vec + vec
	friend vector operator+(vector lhs, const vector &rhs)
	{
		return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
	}
	// vec + diff vec
	template <Number X2, Number Y2, Number Z2>
		requires(!std::same_as<vector<X2, Y2, Z2>, vector>)
	friend vector<X2, Y2, Z2> operator+(vector lhs, const vector<X2, Y2, Z2> &rhs)
	{
		return {
			static_cast<X2>(lhs.x + rhs.x),
			static_cast<Y2>(lhs.y + rhs.y),
			static_cast<Z2>(lhs.z + rhs.z),
		};
	}

	// += other
	template <OtherVector T>
	vector &operator+=(const T &v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}
	// += vec
	friend vector &operator+=(vector &lhs, const vector &rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;

		return lhs;
	}
	// += diff vec
	template <Number X2, Number Y2, Number Z2>
		requires(!std::same_as<vector<X2, Y2, Z2>, vector>)
	friend vector<X2, Y2, Z2> operator+=(vector &lhs, const vector<X2, Y2, Z2> &rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;

		return {
			static_cast<X2>(lhs.x),
			static_cast<Y2>(lhs.y),
			static_cast<Z2>(lhs.z)
		};
	}
};

// other += vec
template <OtherVector T, Number X, Number Y, Number Z>
T &operator+=(T &lhs, const vector<X, Y, Z> &rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;

	return lhs;
}

// MARK: deductions

template <VectorLike T>
vector(const T &v) -> vector<
	std::remove_cvref_t<decltype(v.x)>,
	std::remove_cvref_t<decltype(v.y)>,
	std::remove_cvref_t<decltype(v.z)>>;

template <VectorLike T>
vector(T &&v) -> vector<
	std::remove_cvref_t<decltype(v.x)>,
	std::remove_cvref_t<decltype(v.y)>,
	std::remove_cvref_t<decltype(v.z)>>;

} // namespace vec
