/// @ref core

#include "compute_vector_relational.hpp"

namespace glm
{
	// -- Implicit basic constructors --

#	if GLM_CONFIG_DEFAULTED_FUNCTIONS == GLM_DISABLE
		template<typename T, qualifier Q>
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>::vec()
#			if GLM_CONFIG_CTOR_INIT != GLM_CTOR_INIT_DISABLE
				: x(0), y(0), z(0), w(0), v(0)
#			endif
		{}

		template<typename T, qualifier Q>
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>::vec(vec<5, T, Q> const& v)
			: x(v.x), y(v.y), z(v.z), w(v.w), v(v.v)
		{}
#	endif

	template<typename T, qualifier Q>
	template<qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>::vec(vec<5, T, P> const& v)
		: x(v.x), y(v.y), z(v.z), w(v.w), v(v.v)
	{}

	// -- Explicit basic constructors --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>::vec(T scalar)
		: x(scalar), y(scalar), z(scalar), w(scalar), v(scalar)
	{}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>::vec(T _x, T _y, T _z, T _w, T _v)
		: x(_x), y(_y), z(_z), w(_w), v(_v)
	{}

	// -- Conversion scalar constructors --

	template<typename T, qualifier Q>
	template<typename U, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>::vec(vec<1, U, P> const& v)
		: x(static_cast<T>(v.x))
		, y(static_cast<T>(v.x))
		, z(static_cast<T>(v.x))
		, w(static_cast<T>(v.x))
		, v(static_cast<T>(v.x))
	{}

	template<typename T, qualifier Q>
	template<typename U, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>::vec(vec<5, U, P> const& v)
		: x(static_cast<T>(v.x))
		, y(static_cast<T>(v.y))
		, z(static_cast<T>(v.z))
		, w(static_cast<T>(v.w))
		, v(static_cast<T>(v.v))
	{}

	// -- Conversion vector constructors --
	
	template<typename T, qualifier Q>
	template<typename A, typename B, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>::vec(vec<4, A, P> const& _xyzw, B _v)
		: x(static_cast<T>(_xyzw.x))
		, y(static_cast<T>(_xyzw.y))
		, z(static_cast<T>(_xyzw.z))
		, w(static_cast<T>(_xyzw.w))
		, v(static_cast<T>(_v))
	{}

	// -- Component accesses --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T & vec<5, T, Q>::operator[](typename vec<5, T, Q>::length_type i)
	{
		assert(i >= 0 && i < this->length());
		switch(i)
		{
		default:
			case 0:
		return x;
			case 1:
		return y;
			case 2:
		return z;
			case 3:
		return w;
			case 4:
		return v;
		}
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T const& vec<5, T, Q>::operator[](typename vec<5, T, Q>::length_type i) const
	{
		assert(i >= 0 && i < this->length());
		switch(i)
		{
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		case 4:
			return v;
		}
	}

	// -- Unary arithmetic operators --

#	if GLM_CONFIG_DEFAULTED_FUNCTIONS == GLM_DISABLE
		template<typename T, qualifier Q>
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>& vec<5, T, Q>::operator=(vec<5, T, Q> const& v)
		{
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = v.w;
			this->v = v.v;
			return *this;
		}
#	endif

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q>& vec<5, T, Q>::operator=(vec<5, U, Q> const& v)
	{
		this->x = static_cast<T>(v.x);
		this->y = static_cast<T>(v.y);
		this->z = static_cast<T>(v.z);
		this->w = static_cast<T>(v.w);
		this->v = static_cast<T>(v.v);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator+=(U scalar)
	{
		this->x += static_cast<T>(scalar);
		this->y += static_cast<T>(scalar);
		this->z += static_cast<T>(scalar);
		this->w += static_cast<T>(scalar);
		this->v += static_cast<T>(scalar);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator+=(vec<1, U, Q> const& v)
	{
		this->x += static_cast<T>(v.x);
		this->y += static_cast<T>(v.x);
		this->z += static_cast<T>(v.x);
		this->w += static_cast<T>(v.x);
		this->v += static_cast<T>(v.x);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator+=(vec<5, U, Q> const& v)
	{
		this->x += static_cast<T>(v.x);
		this->y += static_cast<T>(v.y);
		this->z += static_cast<T>(v.z);
		this->w += static_cast<T>(v.w);
		this->v += static_cast<T>(v.v);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator-=(U scalar)
	{
		this->x -= static_cast<T>(scalar);
		this->y -= static_cast<T>(scalar);
		this->z -= static_cast<T>(scalar);
		this->w -= static_cast<T>(scalar);
		this->v -= static_cast<T>(scalar);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator-=(vec<1, U, Q> const& v)
	{
		this->x -= static_cast<T>(v.x);
		this->y -= static_cast<T>(v.x);
		this->z -= static_cast<T>(v.x);
		this->w -= static_cast<T>(v.x);
		this->v -= static_cast<T>(v.x);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator-=(vec<5, U, Q> const& v)
	{
		this->x -= static_cast<T>(v.x);
		this->y -= static_cast<T>(v.y);
		this->z -= static_cast<T>(v.z);
		this->w -= static_cast<T>(v.w);
		this->v -= static_cast<T>(v.v);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator*=(U scalar)
	{
		this->x *= static_cast<T>(scalar);
		this->y *= static_cast<T>(scalar);
		this->z *= static_cast<T>(scalar);
		this->w *= static_cast<T>(scalar);
		this->v *= static_cast<T>(scalar);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator*=(vec<1, U, Q> const& v)
	{
		this->x *= static_cast<T>(v.x);
		this->y *= static_cast<T>(v.x);
		this->z *= static_cast<T>(v.x);
		this->w *= static_cast<T>(v.x);
		this->v *= static_cast<T>(v.x);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator*=(vec<5, U, Q> const& v)
	{
		this->x *= static_cast<T>(v.x);
		this->y *= static_cast<T>(v.y);
		this->z *= static_cast<T>(v.z);
		this->w *= static_cast<T>(v.w);
		this->v *= static_cast<T>(v.v);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator/=(U v)
	{
		this->x /= static_cast<T>(v);
		this->y /= static_cast<T>(v);
		this->z /= static_cast<T>(v);
		this->w /= static_cast<T>(v);
		this->v /= static_cast<T>(v);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator/=(vec<1, U, Q> const& v)
	{
		this->x /= static_cast<T>(v.x);
		this->y /= static_cast<T>(v.x);
		this->z /= static_cast<T>(v.x);
		this->w /= static_cast<T>(v.x);
		this->v /= static_cast<T>(v.x);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator/=(vec<5, U, Q> const& v)
	{
		this->x /= static_cast<T>(v.x);
		this->y /= static_cast<T>(v.y);
		this->z /= static_cast<T>(v.z);
		this->w /= static_cast<T>(v.w);
		this->v /= static_cast<T>(v.v);
		return *this;
	}

	// -- Increment and decrement operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator++()
	{
		++this->x;
		++this->y;
		++this->z;
		++this->w;
		++this->v;
		return *this;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator--()
	{
		--this->x;
		--this->y;
		--this->z;
		--this->w;
		--this->v;
		return *this;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> vec<5, T, Q>::operator++(int)
	{
		vec<5, T, Q> Result(*this);
		++*this;
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> vec<5, T, Q>::operator--(int)
	{
		vec<5, T, Q> Result(*this);
		--*this;
		return Result;
	}

	// -- Unary bit operators --

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator%=(U scalar)
	{
		this->x %= scalar;
		this->y %= scalar;
		this->z %= scalar;
		this->w %= scalar;
		this->v %= scalar;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator%=(vec<1, U, Q> const& v)
	{
		this->x %= v.x;
		this->y %= v.x;
		this->z %= v.x;
		this->w %= v.x;
		this->v %= v.x;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator%=(vec<5, U, Q> const& v)
	{
		this->x %= v.x;
		this->y %= v.y;
		this->z %= v.z;
		this->w %= v.w;
		this->v %= v.v;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator&=(U scalar)
	{
		this->x &= scalar;
		this->y &= scalar;
		this->z &= scalar;
		this->w &= scalar;
		this->v &= scalar;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator&=(vec<1, U, Q> const& v)
	{
		this->x &= v.x;
		this->y &= v.x;
		this->z &= v.x;
		this->w &= v.x;
		this->v &= v.x;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator&=(vec<5, U, Q> const& v)
	{
		this->x &= v.x;
		this->y &= v.y;
		this->z &= v.z;
		this->w &= v.w;
		this->v &= v.v;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator|=(U scalar)
	{
		this->x |= scalar;
		this->y |= scalar;
		this->z |= scalar;
		this->w |= scalar;
		this->v |= scalar;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator|=(vec<1, U, Q> const& v)
	{
		this->x |= v.x;
		this->y |= v.x;
		this->z |= v.x;
		this->w |= v.x;
		this->v |= v.x;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator|=(vec<5, U, Q> const& v)
	{
		this->x |= v.x;
		this->y |= v.y;
		this->z |= v.z;
		this->w |= v.w;
		this->v |= v.v;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator^=(U scalar)
	{
		this->x ^= scalar;
		this->y ^= scalar;
		this->z ^= scalar;
		this->w ^= scalar;
		this->v ^= scalar;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator^=(vec<1, U, Q> const& v)
	{
		this->x ^= v.x;
		this->y ^= v.x;
		this->z ^= v.x;
		this->w ^= v.x;
		this->v ^= v.x;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator^=(vec<5, U, Q> const& v)
	{
		this->x ^= v.x;
		this->y ^= v.y;
		this->z ^= v.z;
		this->w ^= v.w;
		this->v ^= v.v;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator<<=(U scalar)
	{
		this->x <<= scalar;
		this->y <<= scalar;
		this->z <<= scalar;
		this->w <<= scalar;
		this->v <<= scalar;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator<<=(vec<1, U, Q> const& v)
	{
		this->x <<= static_cast<T>(v.x);
		this->y <<= static_cast<T>(v.x);
		this->z <<= static_cast<T>(v.x);
		this->w <<= static_cast<T>(v.x);
		this->v <<= static_cast<T>(v.x);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator<<=(vec<5, U, Q> const& v)
	{
		this->x <<= static_cast<T>(v.x);
		this->y <<= static_cast<T>(v.y);
		this->z <<= static_cast<T>(v.z);
		this->w <<= static_cast<T>(v.w);
		this->v <<= static_cast<T>(v.v);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator>>=(U scalar)
	{
		this->x >>= static_cast<T>(scalar);
		this->y >>= static_cast<T>(scalar);
		this->z >>= static_cast<T>(scalar);
		this->w >>= static_cast<T>(scalar);
		this->v >>= static_cast<T>(scalar);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator>>=(vec<1, U, Q> const& v)
	{
		this->x >>= static_cast<T>(v.x);
		this->y >>= static_cast<T>(v.x);
		this->z >>= static_cast<T>(v.x);
		this->w >>= static_cast<T>(v.x);
		this->v >>= static_cast<T>(v.x);
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> & vec<5, T, Q>::operator>>=(vec<5, U, Q> const& v)
	{
		this->x >>= static_cast<T>(v.x);
		this->y >>= static_cast<T>(v.y);
		this->z >>= static_cast<T>(v.z);
		this->w >>= static_cast<T>(v.w);
		this->v >>= static_cast<T>(v.v);
		return *this;
	}

	// -- Unary arithmetic operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator+(vec<5, T, Q> const& v)
	{
		return v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator-(vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			-v.x,
			-v.y,
			-v.z,
			-v.w,
			-v.v);
	}

	// -- Binary arithmetic operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator+(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x + scalar,
			v.y + scalar,
			v.z + scalar,
			v.w + scalar,
			v.v + scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator+(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x + scalar.x,
			v.y + scalar.x,
			v.z + scalar.x,
			v.w + scalar.x,
			v.v + scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator+(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar + v.x,
			scalar + v.y,
			scalar + v.z,
			scalar + v.w,
			scalar + v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator+(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x + v.x,
			scalar.x + v.y,
			scalar.x + v.z,
			scalar.x + v.w,
			scalar.x + v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator+(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z,
			v1.w + v2.w,
			v1.v + v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator-(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x - scalar,
			v.y - scalar,
			v.z - scalar,
			v.w - scalar,
			v.v - scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator-(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x - scalar.x,
			v.y - scalar.x,
			v.z - scalar.x,
			v.w - scalar.x,
			v.v - scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator-(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar - v.x,
			scalar - v.y,
			scalar - v.z,
			scalar - v.w,
			scalar - v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator-(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x - v.x,
			scalar.x - v.y,
			scalar.x - v.z,
			scalar.x - v.w,
			scalar.x - v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator-(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x - v2.x,
			v1.y - v2.y,
			v1.z - v2.z,
			v1.w - v2.w,
			v1.v - v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator*(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x * scalar,
			v.y * scalar,
			v.z * scalar,
			v.w * scalar,
			v.v * scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator*(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x * scalar.x,
			v.y * scalar.x,
			v.z * scalar.x,
			v.w * scalar.x,
			v.v * scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator*(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar * v.x,
			scalar * v.y,
			scalar * v.z,
			scalar * v.w,
			scalar * v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator*(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x * v.x,
			scalar.x * v.y,
			scalar.x * v.z,
			scalar.x * v.w,
			scalar.x * v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator*(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x * v2.x,
			v1.y * v2.y,
			v1.z * v2.z,
			v1.w * v2.w,
			v1.v * v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator/(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x / scalar,
			v.y / scalar,
			v.z / scalar,
			v.w / scalar,
			v.v / scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator/(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x / scalar.x,
			v.y / scalar.x,
			v.z / scalar.x,
			v.w / scalar.x,
			v.v / scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator/(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar / v.x,
			scalar / v.y,
			scalar / v.z,
			scalar / v.w,
			scalar / v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator/(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x / v.x,
			scalar.x / v.y,
			scalar.x / v.z,
			scalar.x / v.w,
			scalar.x / v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator/(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x / v2.x,
			v1.y / v2.y,
			v1.z / v2.z,
			v1.w / v2.w,
			v1.v / v2.v);
	}

	// -- Binary bit operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator%(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x % scalar,
			v.y % scalar,
			v.z % scalar,
			v.w % scalar,
			v.v % scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator%(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x % scalar.x,
			v.y % scalar.x,
			v.z % scalar.x,
			v.w % scalar.x,
			v.v % scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator%(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar % v.x,
			scalar % v.y,
			scalar % v.z,
			scalar % v.w,
			scalar % v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator%(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x % v.x,
			scalar.x % v.y,
			scalar.x % v.z,
			scalar.x % v.w,
			scalar.x % v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator%(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x % v2.x,
			v1.y % v2.y,
			v1.z % v2.z,
			v1.w % v2.w,
			v1.v % v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator&(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x & scalar,
			v.y & scalar,
			v.z & scalar,
			v.w & scalar,
			v.v & scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator&(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x & scalar.x,
			v.y & scalar.x,
			v.z & scalar.x,
			v.w & scalar.x,
			v.v & scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator&(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar & v.x,
			scalar & v.y,
			scalar & v.z,
			scalar & v.w,
			scalar & v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator&(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x & v.x,
			scalar.x & v.y,
			scalar.x & v.z,
			scalar.x & v.w,
			scalar.x & v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator&(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x & v2.x,
			v1.y & v2.y,
			v1.z & v2.z,
			v1.w & v2.w,
			v1.v & v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator|(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x | scalar,
			v.y | scalar,
			v.z | scalar,
			v.w | scalar,
			v.v | scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator|(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x | scalar.x,
			v.y | scalar.x,
			v.z | scalar.x,
			v.w | scalar.x,
			v.v | scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator|(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar | v.x,
			scalar | v.y,
			scalar | v.z,
			scalar | v.w,
			scalar | v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator|(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x | v.x,
			scalar.x | v.y,
			scalar.x | v.z,
			scalar.x | v.w,
			scalar.x | v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator|(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x | v2.x,
			v1.y | v2.y,
			v1.z | v2.z,
			v1.w | v2.w,
			v1.v | v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator^(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x ^ scalar,
			v.y ^ scalar,
			v.z ^ scalar,
			v.w ^ scalar,
			v.v ^ scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator^(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x ^ scalar.x,
			v.y ^ scalar.x,
			v.z ^ scalar.x,
			v.w ^ scalar.x,
			v.v ^ scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator^(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar ^ v.x,
			scalar ^ v.y,
			scalar ^ v.z,
			scalar ^ v.w,
			scalar ^ v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator^(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x ^ v.x,
			scalar.x ^ v.y,
			scalar.x ^ v.z,
			scalar.x ^ v.w,
			scalar.x ^ v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator^(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x ^ v2.x,
			v1.y ^ v2.y,
			v1.z ^ v2.z,
			v1.w ^ v2.w,
			v1.v ^ v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator<<(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x << scalar,
			v.y << scalar,
			v.z << scalar,
			v.w << scalar,
			v.v << scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator<<(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x << scalar.x,
			v.y << scalar.x,
			v.z << scalar.x,
			v.w << scalar.x,
			v.v << scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator<<(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar << v.x,
			scalar << v.y,
			scalar << v.z,
			scalar << v.w,
			scalar << v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator<<(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x << v.x,
			scalar.x << v.y,
			scalar.x << v.z,
			scalar.x << v.w,
			scalar.x << v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator<<(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x << v2.x,
			v1.y << v2.y,
			v1.z << v2.z,
			v1.w << v2.w,
			v1.v << v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator>>(vec<5, T, Q> const& v, T scalar)
	{
		return vec<5, T, Q>(
			v.x >> scalar,
			v.x >> scalar,
			v.z >> scalar,
			v.w >> scalar,
			v.v >> scalar);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator>>(vec<5, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<5, T, Q>(
			v.x >> scalar.x,
			v.y >> scalar.x,
			v.z >> scalar.x,
			v.w >> scalar.x,
			v.v >> scalar.x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator>>(T scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar >> v.x,
			scalar >> v.y,
			scalar >> v.z,
			scalar >> v.w,
			scalar >> v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator>>(vec<1, T, Q> const& scalar, vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			scalar.x >> v.x,
			scalar.x >> v.y,
			scalar.x >> v.z,
			scalar.x >> v.w,
			scalar.x >> v.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator>>(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return vec<5, T, Q>(
			v1.x >> v2.x,
			v1.y >> v2.y,
			v1.z >> v2.z,
			v1.w >> v2.w,
			v1.v >> v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, T, Q> operator~(vec<5, T, Q> const& v)
	{
		return vec<5, T, Q>(
			~v.x,
			~v.y,
			~v.z,
			~v.w,
			~v.v);
	}

	// -- Boolean operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool operator==(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return
			detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.x, v2.x) &&
			detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.y, v2.y) &&
			detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.z, v2.z) &&
			detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.w, v2.w) &&
			detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.v, v2.v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool operator!=(vec<5, T, Q> const& v1, vec<5, T, Q> const& v2)
	{
		return !(v1 == v2);
	}

	template<qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, bool, Q> operator&&(vec<5, bool, Q> const& v1, vec<5, bool, Q> const& v2)
	{
		return vec<5, bool, Q>(v1.x && v2.x, v1.y && v2.y, v1.z && v2.z, v1.w && v2.w, v1.v && v2.v);
	}

	template<qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<5, bool, Q> operator||(vec<5, bool, Q> const& v1, vec<5, bool, Q> const& v2)
	{
		return vec<5, bool, Q>(v1.x || v2.x, v1.y || v2.y, v1.z || v2.z, v1.w || v2.w, v1.v || v2.v);
	}
}//namespace glm
