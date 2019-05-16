#include "../matrix.hpp"

namespace glm
{
	// -- Constructors --

#	if GLM_CONFIG_DEFAULTED_FUNCTIONS == GLM_DISABLE
		template<typename T, qualifier Q>
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR mat<5, 5, T, Q>::mat()
#			if GLM_CONFIG_CTOR_INIT == GLM_CTOR_INITIALIZER_LIST
				: value{col_type(1, 0, 0, 0, 0), col_type(0, 1, 0, 0, 0), col_type(0, 0, 1, 0, 0), col_type(0, 0, 0, 1, 0), col_type(0, 0, 0, 0, 1) }
#			endif
		{
#			if GLM_CONFIG_CTOR_INIT == GLM_CTOR_INITIALISATION
				this->value[0] = col_type(1, 0, 0, 0, 0);
				this->value[1] = col_type(0, 1, 0, 0, 0);
				this->value[2] = col_type(0, 0, 1, 0, 0);
				this->value[3] = col_type(0, 0, 0, 1, 0);
				this->value[4] = col_type(0, 0, 0, 0, 1);
#			endif
		}
#	endif

	template<typename T, qualifier Q>
	template<qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR mat<5, 5, T, Q>::mat(mat<5, 5, T, P> const& m)
#		if GLM_HAS_INITIALIZER_LISTS
			: value{col_type(m[0]), col_type(m[1]), col_type(m[2]), col_type(m[3]), col_type(m[4])}
#		endif
	{
#		if !GLM_HAS_INITIALIZER_LISTS
			this->value[0] = m[0];
			this->value[1] = m[1];
			this->value[2] = m[2];
			this->value[3] = m[3];
			this->value[4] = m[4];
#		endif
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR mat<5, 5, T, Q>::mat(T const& s)
#		if GLM_HAS_INITIALIZER_LISTS
			: value{col_type(s, 0, 0, 0, 0), col_type(0, s, 0, 0, 0), col_type(0, 0, s, 0, 0), col_type(0, 0, 0, s, 0), col_type(0, 0, 0, 0, s)}
#		endif
	{
#		if !GLM_HAS_INITIALIZER_LISTS
			this->value[0] = col_type(s, 0, 0, 0, 0);
			this->value[1] = col_type(0, s, 0, 0, 0);
			this->value[2] = col_type(0, 0, s, 0, 0);
			this->value[3] = col_type(0, 0, 0, s, 0);
			this->value[4] = col_type(0, 0, 0, 0, s);
#		endif
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR mat<5, 5, T, Q>::mat
	(
		T const& x0, T const& y0, T const& z0, T const& w0, T const& v0,
		T const& x1, T const& y1, T const& z1, T const& w1, T const& v1,
		T const& x2, T const& y2, T const& z2, T const& w2, T const& v2,
		T const& x3, T const& y3, T const& z3, T const& w3, T const& v3,
		T const& x4, T const& y4, T const& z4, T const& w4, T const& v4
	)
#		if GLM_HAS_INITIALIZER_LISTS
			: value{
				col_type(x0, y0, z0, w0, v0),
				col_type(x1, y1, z1, w1, v1),
				col_type(x2, y2, z2, w2, v2),
				col_type(x3, y3, z3, w3, v3),
				col_type(x4, y4, z4, w4, v4)}
#		endif
	{
#		if !GLM_HAS_INITIALIZER_LISTS
			this->value[0] = col_type(x0, y0, z0, w0, v0);
			this->value[1] = col_type(x1, y1, z1, w1, v1);
			this->value[2] = col_type(x2, y2, z2, w2, v2);
			this->value[3] = col_type(x3, y3, z3, w3, v3);
			this->value[4] = col_type(x4, y4, z4, w4, v4);
#		endif
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR mat<5, 5, T, Q>::mat(col_type const& v0, col_type const& v1, col_type const& v2, col_type const& v3, col_type const& v4)
#		if GLM_HAS_INITIALIZER_LISTS
			: value{col_type(v0), col_type(v1), col_type(v2), col_type(v3), col_type(v4)}
#		endif
	{
#		if !GLM_HAS_INITIALIZER_LISTS
			this->value[0] = v0;
			this->value[1] = v1;
			this->value[2] = v2;
			this->value[3] = v3;
			this->value[4] = v4;
#		endif
	}

	template<typename T, qualifier Q>
	template<typename U, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR mat<5, 5, T, Q>::mat(mat<5, 5, U, P> const& m)
#		if GLM_HAS_INITIALIZER_LISTS
			: value{col_type(m[0]), col_type(m[1]), col_type(m[2]), col_type(m[3]), col_type(m[4])}
#		endif
	{
#		if !GLM_HAS_INITIALIZER_LISTS
			this->value[0] = col_type(m[0]);
			this->value[1] = col_type(m[1]);
			this->value[2] = col_type(m[2]);
			this->value[3] = col_type(m[3]);
			this->value[4] = col_type(m[4]);
#		endif
	}

	// -- Conversions --

	template<typename T, qualifier Q>
	template<
		typename X1, typename Y1, typename Z1, typename W1, typename V1,
		typename X2, typename Y2, typename Z2, typename W2, typename V2,
		typename X3, typename Y3, typename Z3, typename W3, typename V3,
		typename X4, typename Y4, typename Z4, typename W4, typename V4,
		typename X5, typename Y5, typename Z5, typename W5, typename V5>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR mat<5, 5, T, Q>::mat
	(
		X1 const& x1, Y1 const& y1, Z1 const& z1, W1 const& w1, V1 const& v1,
		X2 const& x2, Y2 const& y2, Z2 const& z2, W2 const& w2, V2 const& v2,
		X3 const& x3, Y3 const& y3, Z3 const& z3, W3 const& w3, V3 const& v3,
		X4 const& x4, Y4 const& y4, Z4 const& z4, W4 const& w4, V4 const& v4,
		X5 const& x5, Y5 const& y5, Z5 const& z5, W5 const& w5, W5 const& v5
	)
#		if GLM_HAS_INITIALIZER_LISTS
			: value{col_type(x1, y1, z1, w1, v1), col_type(x2, y2, z2, w2, v2), col_type(x3, y3, z3, w3, v3), col_type(x4, y4, z4, w4, v4), col_type(x5, y5, z5, w5, v5)}
#		endif
	{
		GLM_STATIC_ASSERT(std::numeric_limits<X1>::is_iec559 || std::numeric_limits<X1>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 1st parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<Y1>::is_iec559 || std::numeric_limits<Y1>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 2nd parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<Z1>::is_iec559 || std::numeric_limits<Z1>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 3rd parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<W1>::is_iec559 || std::numeric_limits<W1>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 4th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<V1>::is_iec559 || std::numeric_limits<V1>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 5th parameter type invalid.");

		GLM_STATIC_ASSERT(std::numeric_limits<X2>::is_iec559 || std::numeric_limits<X2>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 6th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<Y2>::is_iec559 || std::numeric_limits<Y2>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 7th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<Z2>::is_iec559 || std::numeric_limits<Z2>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 8th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<W2>::is_iec559 || std::numeric_limits<W2>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 9th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<V2>::is_iec559 || std::numeric_limits<V2>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 10th parameter type invalid.");

		GLM_STATIC_ASSERT(std::numeric_limits<X3>::is_iec559 || std::numeric_limits<X3>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 11th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<Y3>::is_iec559 || std::numeric_limits<Y3>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 12th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<Z3>::is_iec559 || std::numeric_limits<Z3>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 13th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<W3>::is_iec559 || std::numeric_limits<W3>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 14th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<V3>::is_iec559 || std::numeric_limits<V3>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 15th parameter type invalid.");

		GLM_STATIC_ASSERT(std::numeric_limits<X4>::is_iec559 || std::numeric_limits<X4>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 16th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<Y4>::is_iec559 || std::numeric_limits<Y4>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 17th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<Z4>::is_iec559 || std::numeric_limits<Z4>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 18th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<W4>::is_iec559 || std::numeric_limits<W4>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 19th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<V4>::is_iec559 || std::numeric_limits<V4>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 20th parameter type invalid.");

#		if !GLM_HAS_INITIALIZER_LISTS
			this->value[0] = col_type(x1, y1, z1, w1, v1);
			this->value[1] = col_type(x2, y2, z2, w2, v2);
			this->value[2] = col_type(x3, y3, z3, w3, v3);
			this->value[3] = col_type(x4, y4, z4, w4, v4);
			this->value[4] = col_type(x5, y5, z5, w5, v5);
#		endif
	}

	template<typename T, qualifier Q>
	template<typename V1, typename V2, typename V3, typename V4, typename V5>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR mat<5, 5, T, Q>::mat(vec<5, V1, Q> const& v1, vec<5, V2, Q> const& v2, vec<5, V3, Q> const& v3, vec<5, V4, Q> const& v4, vec<5, V5, Q> const& v5)
#		if GLM_HAS_INITIALIZER_LISTS
			: value{col_type(v1), col_type(v2), col_type(v3), col_type(v4)}
#		endif
	{
		GLM_STATIC_ASSERT(std::numeric_limits<V1>::is_iec559 || std::numeric_limits<V1>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 1st parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<V2>::is_iec559 || std::numeric_limits<V2>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 2nd parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<V3>::is_iec559 || std::numeric_limits<V3>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 3rd parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<V4>::is_iec559 || std::numeric_limits<V4>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 4th parameter type invalid.");
		GLM_STATIC_ASSERT(std::numeric_limits<V5>::is_iec559 || std::numeric_limits<V5>::is_integer || GLM_CONFIG_UNRESTRICTED_GENTYPE, "*mat5x5 constructor only takes float and integer types, 5th parameter type invalid.");

#		if !GLM_HAS_INITIALIZER_LISTS
			this->value[0] = col_type(v1);
			this->value[1] = col_type(v2);
			this->value[2] = col_type(v3);
			this->value[3] = col_type(v4);
			this->value[4] = col_type(v5);
#		endif
	}

	// -- Matrix conversions --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR mat<5, 5, T, Q>::mat(mat<4, 4, T, Q> const& m)
#		if GLM_HAS_INITIALIZER_LISTS
			: value{col_type(m[0], 0), col_type(m[1], 0), col_type(m[2], 0), col_type(m[3], 0), col_type(0, 0, 0, 0, 1)}
#		endif
	{
#		if !GLM_HAS_INITIALIZER_LISTS
			this->value[0] = col_type(m[0], 0);
			this->value[1] = col_type(m[1], 0);
			this->value[2] = col_type(m[2], 0);
			this->value[3] = col_type(m[3], 0);
			this->value[4] = col_type(0, 0, 0, 0, 1);
#		endif
	}

	// -- Accesses --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER typename mat<5, 5, T, Q>::col_type & mat<5, 5, T, Q>::operator[](typename mat<5, 5, T, Q>::length_type i)
	{
		assert(i < this->length());
		return this->value[i];
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR typename mat<5, 5, T, Q>::col_type const& mat<5, 5, T, Q>::operator[](typename mat<5, 5, T, Q>::length_type i) const
	{
		assert(i < this->length());
		return this->value[i];
	}

	// -- Unary arithmetic operators --

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q>& mat<5, 5, T, Q>::operator=(mat<5, 5, U, Q> const& m)
	{
		//memcpy could be faster
		//memcpy(&this->value, &m.value, 16 * sizeof(valType));
		this->value[0] = m[0];
		this->value[1] = m[1];
		this->value[2] = m[2];
		this->value[3] = m[3];
		this->value[4] = m[4];
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q>& mat<5, 5, T, Q>::operator+=(U s)
	{
		this->value[0] += s;
		this->value[1] += s;
		this->value[2] += s;
		this->value[3] += s;
		this->value[4] += s;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q>& mat<5, 5, T, Q>::operator+=(mat<5, 5, U, Q> const& m)
	{
		this->value[0] += m[0];
		this->value[1] += m[1];
		this->value[2] += m[2];
		this->value[3] += m[3];
		this->value[4] += m[4];
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> & mat<5, 5, T, Q>::operator-=(U s)
	{
		this->value[0] -= s;
		this->value[1] -= s;
		this->value[2] -= s;
		this->value[3] -= s;
		this->value[4] -= s;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> & mat<5, 5, T, Q>::operator-=(mat<5, 5, U, Q> const& m)
	{
		this->value[0] -= m[0];
		this->value[1] -= m[1];
		this->value[2] -= m[2];
		this->value[3] -= m[3];
		this->value[4] -= m[4];
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> & mat<5, 5, T, Q>::operator*=(U s)
	{
		this->value[0] *= s;
		this->value[1] *= s;
		this->value[2] *= s;
		this->value[3] *= s;
		this->value[4] *= s;
		return *this;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> & mat<5, 5, T, Q>::operator*=(mat<5, 5, U, Q> const& m)
	{
		return (*this = *this * m);
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> & mat<5, 5, T, Q>::operator/=(U s)
	{
		this->value[0] /= s;
		this->value[1] /= s;
		this->value[2] /= s;
		this->value[3] /= s;
		this->value[4] /= s;
		return *this;
	}

	// -- Increment and decrement operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> & mat<5, 5, T, Q>::operator++()
	{
		++this->value[0];
		++this->value[1];
		++this->value[2];
		++this->value[3];
		++this->value[4];
		return *this;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> & mat<5, 5, T, Q>::operator--()
	{
		--this->value[0];
		--this->value[1];
		--this->value[2];
		--this->value[3];
		--this->value[4];
		return *this;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> mat<5, 5, T, Q>::operator++(int)
	{
		mat<5, 5, T, Q> Result(*this);
		++*this;
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> mat<5, 5, T, Q>::operator--(int)
	{
		mat<5, 5, T, Q> Result(*this);
		--*this;
		return Result;
	}

	// -- Unary constant operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator+(mat<5, 5, T, Q> const& m)
	{
		return m;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator-(mat<5, 5, T, Q> const& m)
	{
		return mat<5, 5, T, Q>(
			-m[0],
			-m[1],
			-m[2],
			-m[3],
			-m[4]);
	}

	// -- Binary arithmetic operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator+(mat<5, 5, T, Q> const& m, T const& s)
	{
		return mat<5, 5, T, Q>(
			m[0] + s,
			m[1] + s,
			m[2] + s,
			m[3] + s);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator+(T const& s, mat<5, 5, T, Q> const& m)
	{
		return mat<5, 5, T, Q>(
			m[0] + s,
			m[1] + s,
			m[2] + s,
			m[3] + s,
			m[4] + s);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator+(mat<5, 5, T, Q> const& m1, mat<5, 5, T, Q> const& m2)
	{
		return mat<5, 5, T, Q>(
			m1[0] + m2[0],
			m1[1] + m2[1],
			m1[2] + m2[2],
			m1[3] + m2[3],
			m1[4] + m2[4]);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator-(mat<5, 5, T, Q> const& m, T const& s)
	{
		return mat<5, 5, T, Q>(
			m[0] - s,
			m[1] - s,
			m[2] - s,
			m[3] - s,
			m[4] - s);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator-(T const& s, mat<5, 5, T, Q> const& m)
	{
		return mat<5, 5, T, Q>(
			s - m[0],
			s - m[1],
			s - m[2],
			s - m[3],
			s - m[4]);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator-(mat<5, 5, T, Q> const& m1, mat<5, 5, T, Q> const& m2)
	{
		return mat<5, 5, T, Q>(
			m1[0] - m2[0],
			m1[1] - m2[1],
			m1[2] - m2[2],
			m1[3] - m2[3],
			m1[4] - m2[4]);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator*(mat<5, 5, T, Q> const& m, T const  & s)
	{
		return mat<5, 5, T, Q>(
			m[0] * s,
			m[1] * s,
			m[2] * s,
			m[3] * s,
			m[4] * s);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator*(T const& s, mat<5, 5, T, Q> const& m)
	{
		return mat<5, 5, T, Q>(
			m[0] * s,
			m[1] * s,
			m[2] * s,
			m[3] * s,
			m[4] * s);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER typename mat<5, 5, T, Q>::col_type operator*
	(
		mat<5, 5, T, Q> const& m,
		typename mat<5, 5, T, Q>::row_type const& v
	)
	{
		typename mat<5, 5, T, Q>::col_type const Mov0(v[0]);
		typename mat<5, 5, T, Q>::col_type const Mov1(v[1]);
		typename mat<5, 5, T, Q>::col_type const Mul0 = m[0] * Mov0;
		typename mat<5, 5, T, Q>::col_type const Mul1 = m[1] * Mov1;
		typename mat<5, 5, T, Q>::col_type const Add0 = Mul0 + Mul1;
		typename mat<5, 5, T, Q>::col_type const Mov2(v[2]);
		typename mat<5, 5, T, Q>::col_type const Mov3(v[3]);
		typename mat<5, 5, T, Q>::col_type const Mul2 = m[2] * Mov2;
		typename mat<5, 5, T, Q>::col_type const Mul3 = m[3] * Mov3;
		typename mat<5, 5, T, Q>::col_type const Add1 = Mul2 + Mul3;
		typename mat<5, 5, T, Q>::col_type const Mov4(v[4]);
		typename mat<5, 5, T, Q>::col_type const Mul4 = m[4] * Mov4;
		typename mat<5, 5, T, Q>::col_type const Add2 = Add0 + Add1 + Mul4;
		return Add2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER typename mat<4, 4, T, Q>::col_type operator*
	(
		mat<5, 5, T, Q> const& m,
		typename mat<4, 4, T, Q>::row_type const& v
	)
	{
		typename mat<4, 4, T, Q>::col_type const Mov0(v[0]);
		typename mat<4, 4, T, Q>::col_type const Mov1(v[1]);
		typename mat<4, 4, T, Q>::col_type const Mul0 = mat<4, 4, T, Q>::col_type(m[0].x, m[0].y, m[0].z, m[0].w) * Mov0;
		typename mat<4, 4, T, Q>::col_type const Mul1 = mat<4, 4, T, Q>::col_type(m[1].x, m[1].y, m[1].z, m[1].w) * Mov1;
		typename mat<4, 4, T, Q>::col_type const Add0 = Mul0 + Mul1;
		typename mat<4, 4, T, Q>::col_type const Mov2(v[2]);
		typename mat<4, 4, T, Q>::col_type const Mov3(v[3]);
		typename mat<4, 4, T, Q>::col_type const Mul2 = mat<4, 4, T, Q>::col_type(m[2].x, m[2].y, m[2].z, m[2].w) * Mov2;
		typename mat<4, 4, T, Q>::col_type const Mul3 = mat<4, 4, T, Q>::col_type(m[3].x, m[3].y, m[3].z, m[3].w) * Mov3;
		typename mat<4, 4, T, Q>::col_type const Add1 = Mul2 + Mul3;
		typename mat<4, 4, T, Q>::col_type const Add2 = Add0 + Add1;
		return Add2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER typename mat<5, 5, T, Q>::row_type operator*
	(
		typename mat<5, 5, T, Q>::col_type const& v,
		mat<5, 5, T, Q> const& m
	)
	{
		return typename mat<5, 5, T, Q>::row_type(
			m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3] + m[0][4] * v[4],
			m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3] + m[1][4] * v[4],
			m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3] + m[2][4] * v[4],
			m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3] * v[3] + m[3][4] * v[4],
			m[4][0] * v[0] + m[4][1] * v[1] + m[4][2] * v[2] + m[4][3] * v[3] + m[4][4] * v[4]);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator*(mat<5, 5, T, Q> const& m1, mat<5, 5, T, Q> const& m2)
	{
		typename mat<5, 5, T, Q>::col_type const SrcA0 = m1[0];
		typename mat<5, 5, T, Q>::col_type const SrcA1 = m1[1];
		typename mat<5, 5, T, Q>::col_type const SrcA2 = m1[2];
		typename mat<5, 5, T, Q>::col_type const SrcA3 = m1[3];
		typename mat<5, 5, T, Q>::col_type const SrcA4 = m1[4];

		typename mat<5, 5, T, Q>::col_type const SrcB0 = m2[0];
		typename mat<5, 5, T, Q>::col_type const SrcB1 = m2[1];
		typename mat<5, 5, T, Q>::col_type const SrcB2 = m2[2];
		typename mat<5, 5, T, Q>::col_type const SrcB3 = m2[3];
		typename mat<5, 5, T, Q>::col_type const SrcB4 = m2[4];

		mat<5, 5, T, Q> Result;
		Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3] + SrcA4 * SrcB0[4];
		Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3] + SrcA4 * SrcB1[4];
		Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3] + SrcA4 * SrcB2[4];
		Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3] + SrcA4 * SrcB3[4];
		Result[4] = SrcA0 * SrcB4[0] + SrcA1 * SrcB4[1] + SrcA2 * SrcB4[2] + SrcA3 * SrcB4[3] + SrcA4 * SrcB4[4];
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator/(mat<5, 5, T, Q> const& m, T const& s)
	{
		return mat<5, 5, T, Q>(
			m[0] / s,
			m[1] / s,
			m[2] / s,
			m[3] / s,
			m[4] / s);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<5, 5, T, Q> operator/(T const& s,	mat<5, 5, T, Q> const& m)
	{
		return mat<5, 5, T, Q>(
			s / m[0],
			s / m[1],
			s / m[2],
			s / m[3],
			s / m[4]);
	}

	// -- Boolean operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER bool operator==(mat<5, 5, T, Q> const& m1, mat<5, 5, T, Q> const& m2)
	{
		return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]) && (m1[3] == m2[3]) && (m1[4] == m2[4]);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER bool operator!=(mat<5, 5, T, Q> const& m1, mat<5, 5, T, Q> const& m2)
	{
		return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]) || (m1[3] != m2[3]) || (m1[4] != m2[4]);
	}
}//namespace glm

