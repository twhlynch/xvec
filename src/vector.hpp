#pragma once

#include <concepts>
#include <type_traits>

namespace xvec {

// MARK: concepts

// integral or floating point (includes bool and char types)
template <typename T>
concept Number =
	std::integral<std::remove_cvref_t<T>> || std::floating_point<std::remove_cvref_t<T>>;

template <typename T>
concept VectorLike = requires(const T vec) {
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

// MARK: operators macro

// vec OP other, other OP vec, other OP derived vec, vec OP vec, vec OP diff
// vec, derived vec OP vec, vec COMPOUND other, vec COMPOUND vec, vec COMPOUND
// diff vec, other COMPOUND vec
#define VEC_DEFINE_ARITHMETIC_OPERATORS(OP, COMPOUND)                                                                \
                                                                                                                     \
	/* vec OP other */                                                                                               \
	template <Vector L, OtherVector T>                                                                               \
		requires(std::derived_from<std::remove_cvref_t<L>, vector>)                                                  \
	[[nodiscard]] constexpr friend std::remove_cvref_t<L>                                                            \
	operator OP(L lhs, const T &rhs) noexcept                                                                        \
	{                                                                                                                \
		return {                                                                                                     \
			static_cast<std::remove_cvref_t<decltype(lhs.x)>>(lhs.x OP rhs.x),                                       \
			static_cast<std::remove_cvref_t<decltype(lhs.y)>>(lhs.y OP rhs.y),                                       \
			static_cast<std::remove_cvref_t<decltype(lhs.z)>>(lhs.z OP rhs.z),                                       \
		};                                                                                                           \
	}                                                                                                                \
                                                                                                                     \
	/* other OP vec */                                                                                               \
	template <OtherVector T>                                                                                         \
	[[nodiscard]] constexpr friend T                                                                                 \
	operator OP(T lhs, const vector &rhs) noexcept                                                                   \
	{                                                                                                                \
		return {                                                                                                     \
			static_cast<std::remove_cvref_t<decltype(lhs.x)>>(lhs.x OP rhs.x),                                       \
			static_cast<std::remove_cvref_t<decltype(lhs.y)>>(lhs.y OP rhs.y),                                       \
			static_cast<std::remove_cvref_t<decltype(lhs.z)>>(lhs.z OP rhs.z),                                       \
		};                                                                                                           \
	}                                                                                                                \
                                                                                                                     \
	/* other OP derived vec */                                                                                       \
	template <OtherVector T, Vector R>                                                                               \
		requires(!std::same_as<std::remove_cvref_t<R>, vector> && std::derived_from<std::remove_cvref_t<R>, vector>) \
	[[nodiscard]] constexpr friend std::remove_cvref_t<R>                                                            \
	operator OP(T lhs, const R &rhs) noexcept                                                                        \
	{                                                                                                                \
		return {                                                                                                     \
			static_cast<std::remove_cvref_t<decltype(rhs.x)>>(lhs.x OP rhs.x),                                       \
			static_cast<std::remove_cvref_t<decltype(rhs.y)>>(lhs.y OP rhs.y),                                       \
			static_cast<std::remove_cvref_t<decltype(rhs.z)>>(lhs.z OP rhs.z),                                       \
		};                                                                                                           \
	}                                                                                                                \
                                                                                                                     \
	/* vec OP vec (same) */                                                                                          \
	[[nodiscard]] constexpr friend vector                                                                            \
	operator OP(vector lhs, const vector &rhs) noexcept                                                              \
	{                                                                                                                \
		return {lhs.x OP rhs.x, lhs.y OP rhs.y, lhs.z OP rhs.z};                                                     \
	}                                                                                                                \
                                                                                                                     \
	/* vec OP diff vec */                                                                                            \
	template <Number X2, Number Y2, Number Z2>                                                                       \
	[[nodiscard]] constexpr friend vector<X2, Y2, Z2>                                                                \
	operator OP(vector lhs, const vector<X2, Y2, Z2> &rhs) noexcept                                                  \
	{                                                                                                                \
		return {                                                                                                     \
			static_cast<X2>(lhs.x OP rhs.x),                                                                         \
			static_cast<Y2>(lhs.y OP rhs.y),                                                                         \
			static_cast<Z2>(lhs.z OP rhs.z),                                                                         \
		};                                                                                                           \
	}                                                                                                                \
                                                                                                                     \
	/* derived vec OP vec */                                                                                         \
	template <Vector L, Vector R>                                                                                    \
		requires(!std::same_as<std::remove_cvref_t<L>, vector> && std::derived_from<std::remove_cvref_t<L>, vector>) \
	[[nodiscard]] constexpr friend std::remove_cvref_t<L>                                                            \
	operator OP(L lhs, const R &rhs) noexcept                                                                        \
	{                                                                                                                \
		return {                                                                                                     \
			static_cast<std::remove_cvref_t<decltype(lhs.x)>>(lhs.x OP rhs.x),                                       \
			static_cast<std::remove_cvref_t<decltype(lhs.y)>>(lhs.y OP rhs.y),                                       \
			static_cast<std::remove_cvref_t<decltype(lhs.z)>>(lhs.z OP rhs.z),                                       \
		};                                                                                                           \
	}                                                                                                                \
                                                                                                                     \
	/* vec COMPOUND other */                                                                                         \
	template <OtherVector T>                                                                                         \
	constexpr vector &                                                                                               \
	operator COMPOUND(const T &v) noexcept                                                                           \
	{                                                                                                                \
		this->x COMPOUND v.x;                                                                                        \
		this->y COMPOUND v.y;                                                                                        \
		this->z COMPOUND v.z;                                                                                        \
		return *this;                                                                                                \
	}                                                                                                                \
                                                                                                                     \
	/* vec COMPOUND vec (same) */                                                                                    \
	constexpr friend vector &                                                                                        \
	operator COMPOUND(vector &lhs, const vector &rhs) noexcept                                                       \
	{                                                                                                                \
		lhs.x COMPOUND rhs.x;                                                                                        \
		lhs.y COMPOUND rhs.y;                                                                                        \
		lhs.z COMPOUND rhs.z;                                                                                        \
		return lhs;                                                                                                  \
	}                                                                                                                \
                                                                                                                     \
	/* vec COMPOUND diff vec */                                                                                      \
	template <Number X2, Number Y2, Number Z2>                                                                       \
	constexpr friend vector<X2, Y2, Z2>                                                                              \
	operator COMPOUND(vector &lhs, const vector<X2, Y2, Z2> &rhs) noexcept                                           \
	{                                                                                                                \
		lhs.x COMPOUND rhs.x;                                                                                        \
		lhs.y COMPOUND rhs.y;                                                                                        \
		lhs.z COMPOUND rhs.z;                                                                                        \
		return {                                                                                                     \
			static_cast<X2>(lhs.x),                                                                                  \
			static_cast<Y2>(lhs.y),                                                                                  \
			static_cast<Z2>(lhs.z)                                                                                   \
		};                                                                                                           \
	}

// MARK: comparison operators

#define VEC_DEFINE_COMPARISON(OP)                                   \
	/* vec OP other */                                              \
	template <OtherVector T>                                        \
	[[nodiscard]] constexpr friend bool                             \
	operator OP(vector lhs, const T &rhs) noexcept                  \
	{                                                               \
		return lhs.x OP rhs.x && lhs.y OP rhs.y && lhs.z OP rhs.z;  \
	}                                                               \
                                                                    \
	/* other OP vec */                                              \
	template <OtherVector T>                                        \
	[[nodiscard]] constexpr friend bool                             \
	operator OP(T lhs, const vector &rhs) noexcept                  \
	{                                                               \
		return lhs.x OP rhs.x && lhs.y OP rhs.y && lhs.z OP rhs.z;  \
	}                                                               \
                                                                    \
	/* vec OP vec (same) */                                         \
	[[nodiscard]] constexpr friend bool                             \
	operator OP(vector lhs, const vector &rhs) noexcept             \
	{                                                               \
		return lhs.x OP rhs.x && lhs.y OP rhs.y && lhs.z OP rhs.z;  \
	}                                                               \
                                                                    \
	/* vec OP diff vec */                                           \
	template <Number X2, Number Y2, Number Z2>                      \
	[[nodiscard]] constexpr friend bool                             \
	operator OP(vector lhs, const vector<X2, Y2, Z2> &rhs) noexcept \
	{                                                               \
		return lhs.x OP rhs.x && lhs.y OP rhs.y && lhs.z OP rhs.z;  \
	}

// MARK: class

template <Number X, Number Y, Number Z>
struct vector
{
	X x {};
	Y y {};
	Z z {};

	// constructors
	[[nodiscard]] constexpr vector() noexcept = default;
	[[nodiscard]] constexpr vector(X x, Y y, Z z) noexcept : x(x), y(y), z(z) {}
	template <Number T>
	[[nodiscard]] constexpr vector(T n) noexcept : x(n), y(n), z(n)
	{
	}
	template <VectorLike T>
	[[nodiscard]] constexpr vector(T v) noexcept : x(v.x), y(v.y), z(v.z)
	{
	}

	// MARK: methods

	template <VectorLike T>
	constexpr vector &operator=(T v) noexcept
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	template <VectorLike T>
	[[nodiscard]] constexpr
	operator T() const noexcept
	{ return {x, y, z}; }

	VEC_DEFINE_ARITHMETIC_OPERATORS(+, +=)
	VEC_DEFINE_ARITHMETIC_OPERATORS(-, -=)
	VEC_DEFINE_ARITHMETIC_OPERATORS(*, *=)
	VEC_DEFINE_ARITHMETIC_OPERATORS(/, /=)

	VEC_DEFINE_COMPARISON(==)
	VEC_DEFINE_COMPARISON(!=)
	VEC_DEFINE_COMPARISON(>)
	VEC_DEFINE_COMPARISON(<)
	VEC_DEFINE_COMPARISON(>=)
	VEC_DEFINE_COMPARISON(<=)

	[[nodiscard]] constexpr friend bool
	operator!(vector v) noexcept
	{
		return !v.x && !v.y && !v.z;
	}
};

#undef VEC_DEFINE_ARITHMETIC_OPERATORS
#undef VEC_DEFINE_COMPARISON

// MARK: other COMPOUND vec

#define VEC_DEFINE_OTHER_COMPOUND(COMPOUND)                        \
	template <OtherVector T, Number X, Number Y, Number Z>         \
	constexpr auto &                                               \
	operator COMPOUND(T &lhs, const vector<X, Y, Z> &rhs) noexcept \
	{                                                              \
		lhs.x COMPOUND rhs.x;                                      \
		lhs.y COMPOUND rhs.y;                                      \
		lhs.z COMPOUND rhs.z;                                      \
		return lhs;                                                \
	}

VEC_DEFINE_OTHER_COMPOUND(+=)
VEC_DEFINE_OTHER_COMPOUND(-=)
VEC_DEFINE_OTHER_COMPOUND(*=)
VEC_DEFINE_OTHER_COMPOUND(/=)

#undef VEC_DEFINE_OTHER_COMPOUND

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

} // namespace xvec
