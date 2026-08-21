#include <cmath>
#include <cstdlib>

#include "tests.hpp"
#include "vector.hpp"

// simple 3D vector with some math member functions inheriting from
// xvec::vector provides construction, arithmetic, and comparison with every
// other vector like type for free

template <xvec::Number X, xvec::Number Y, xvec::Number Z>
struct Vec3 : xvec::vector<X, Y, Z>
{
	// dependent bases are not searched by unqualified lookup so bring the
	// components into scope manually
	using xvec::vector<X, Y, Z>::vector;
	using xvec::vector<X, Y, Z>::x;
	using xvec::vector<X, Y, Z>::y;
	using xvec::vector<X, Y, Z>::z;

	[[nodiscard]] constexpr auto dot(const Vec3 &rhs) const noexcept
	{ return (x * rhs.x) + (y * rhs.y) + (z * rhs.z); }

	[[nodiscard]] constexpr auto
	length_squared() const noexcept
	{ return dot(*this); }

	[[nodiscard]] auto
	length() const noexcept
	{ return std::sqrt(length_squared()); }

	[[nodiscard]] auto
	normalized() const noexcept
	{ return *this / length(); }
};

// deduction guides do not carry over from the base class sadly
template <xvec::Number X, xvec::Number Y, xvec::Number Z>
Vec3(X, Y, Z) -> Vec3<X, Y, Z>;

// some other vector, maybe from a third party library we cannot change

struct OtherVector
{
	float x;
	float y;
	float z;
};

// all our math works ootb with OtherVector even though it is never mentioned
// anywhere in the vector implementation or in xvec::vector

int main()
{
	// ball with gravity, wind, drag, and restitution

	constexpr float dt = 0.05f;
	constexpr float restitution = 0.8f;
	constexpr float drag = 0.01f;
	constexpr int steps = 20;

	Vec3 gravity {0.0f, 0.0f, -9.8f};
	OtherVector wind {.x = 0.25f, .y = 0.1f, .z = 0.0f}; // not our vector
	Vec3 position {0.0f, 0.0f, 1.0f};
	const Vec3 launch {1.5f, 0.5f, 0.0f};
	Vec3 velocity = launch + wind; // Vec3 + OtherVector through xvec::vector

	// dot accepts wind despite the parameter being a Vec3 because it converts
	// OtherVector to Vec3 through xvec
	static_cast<void>(position.dot(wind));

	for (int step = 0; step < steps; ++step)
	{
		// Vec3 - OtherVector, result is a Vec3
		const auto air_relative {velocity - wind};
		const float speed_squared = air_relative.length_squared();
		if (speed_squared > 0.0f)
		{
			// -= and * float both from xvec::vector
			velocity -= air_relative.normalized() * (drag * speed_squared * dt);
		}

		// += from xvec::vector
		velocity += gravity * dt;
		position += velocity * dt;

		if (position.z <= 0.0f && velocity.z < 0.0f)
		{
			position.z = 0.0f;
			velocity.z = -velocity.z * restitution;
		}
	}

	test();

	return EXIT_SUCCESS;
}
