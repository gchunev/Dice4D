#include "../exponential.hpp"
#include "../common.hpp"

namespace glm{
namespace detail
{
	template<length_t L, typename T, qualifier Q, bool Aligned>
	struct compute_length
	{
		GLM_FUNC_QUALIFIER static T call(vec<L, T, Q> const& v)
		{
			return sqrt(dot(v, v));
		}
	};

	template<length_t L, typename T, qualifier Q, bool Aligned>
	struct compute_distance
	{
		GLM_FUNC_QUALIFIER static T call(vec<L, T, Q> const& p0, vec<L, T, Q> const& p1)
		{
			return length(p1 - p0);
		}
	};

	template<typename V, typename T, bool Aligned>
	struct compute_dot{};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_dot<vec<1, T, Q>, T, Aligned>
	{
		GLM_FUNC_QUALIFIER static T call(vec<1, T, Q> const& a, vec<1, T, Q> const& b)
		{
			return a.x * b.x;
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_dot<vec<2, T, Q>, T, Aligned>
	{
		GLM_FUNC_QUALIFIER static T call(vec<2, T, Q> const& a, vec<2, T, Q> const& b)
		{
			vec<2, T, Q> tmp(a * b);
			return tmp.x + tmp.y;
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_dot<vec<3, T, Q>, T, Aligned>
	{
		GLM_FUNC_QUALIFIER static T call(vec<3, T, Q> const& a, vec<3, T, Q> const& b)
		{
			vec<3, T, Q> tmp(a * b);
			return tmp.x + tmp.y + tmp.z;
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_dot<vec<4, T, Q>, T, Aligned>
	{
		GLM_FUNC_QUALIFIER static T call(vec<4, T, Q> const& a, vec<4, T, Q> const& b)
		{
			vec<4, T, Q> tmp(a * b);
			return (tmp.x + tmp.y) + (tmp.z + tmp.w);
		}
	};

	template<length_t N, typename T, qualifier Q, bool Aligned>
	struct compute_cross {};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_cross<2, T, Q, Aligned>
	{
		GLM_FUNC_QUALIFIER static vec<2, T, Q> call(vec<2, T, Q> const& x)
		{
			return vec<2, T, Q>(-x.y, x.x);
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_cross<3, T, Q, Aligned>
	{
		GLM_FUNC_QUALIFIER static vec<3, T, Q> call(vec<3, T, Q> const& x, vec<3, T, Q> const& y)
		{
			return vec<3, T, Q>(
				x[1] * y[2] - y[1] * x[2],
				x[2] * y[0] - y[2] * x[0],
				x[0] * y[1] - y[0] * x[1]
			);
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_cross<4, T, Q, Aligned>
	{
		GLM_FUNC_QUALIFIER static vec<4, T, Q> call(vec<4, T, Q> const& x, vec<4, T, Q> const& y, vec<4, T, Q> const& z)
		{
			return vec<4, T, Q>(
				 x[3] * y[2] * z[1] - x[2] * y[3] * z[1] - x[3] * y[1] * z[2] + x[1] * y[3] * z[2] + x[2] * y[1] * z[3] - x[1] * y[2] * z[3],
				-x[3] * y[2] * z[0] + x[2] * y[3] * z[0] + x[3] * y[0] * z[2] - x[0] * y[3] * z[2] - x[2] * y[0] * z[3] + x[0] * y[2] * z[3],
				 x[3] * y[1] * z[0] - x[1] * y[3] * z[0] - x[3] * y[0] * z[1] + x[0] * y[3] * z[1] + x[1] * y[0] * z[3] - x[0] * y[1] * z[3],
				-x[2] * y[1] * z[0] + x[1] * y[2] * z[0] + x[2] * y[0] * z[1] - x[0] * y[2] * z[1] - x[1] * y[0] * z[2] + x[0] * y[1] * z[2]
				);
		}
	};

	template<length_t N, typename T, qualifier Q, bool Aligned>
	struct compute_cross_as_tensor {};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_cross_as_tensor<2, T, Q, Aligned>
	{
		GLM_FUNC_QUALIFIER static mat<2, 2, T, Q> call(vec<2, T, Q> const& x, vec<2, T, Q> const& y)
		{
			// 2d cross product as tensor

			const auto xy = -x[1] * y[0] + x[0] * y[1];

			return mat<2, 2, T, Q>(0, -xy, 0, xy);
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_cross_as_tensor<3, T, Q, Aligned>
	{
		GLM_FUNC_QUALIFIER static mat<3, 3, T, Q> call(vec<3, T, Q> const& x, vec<3, T, Q> const& y)
		{
			const auto v = detail::compute_cross<3, T, Q, Aligned>::call(x, y);

			return mat<3, 3, T, Q>(v[0], v[1], v[2]);
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_cross_as_tensor<4, T, Q, Aligned>
	{
		GLM_FUNC_QUALIFIER static mat<4, 4, T, Q> call(vec<4, T, Q> const& x, vec<4, T, Q> const& y)
		{
			const auto xy = - x[1] * y[0] + x[0] * y[1];
			const auto xz =   x[2] * y[0] - x[0] * y[2];
			const auto xw = - x[3] * y[0] + x[0] * y[3];
			const auto yz = - x[2] * y[1] + x[1] * y[2];
			const auto yw =   x[3] * y[1] - x[1] * y[3];
			const auto zw = - x[3] * y[2] + x[2] * y[3];

			return mat<4, 4, T, Q>(xy, xz, xw, yz, yw, zw);
		}
	};

	template<length_t L, typename T, qualifier Q, bool Aligned>
	struct compute_normalize
	{
		GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& v)
		{
			return v * inversesqrt(dot(v, v));
		}
	};

	template<length_t L, typename T, qualifier Q, bool Aligned>
	struct compute_faceforward
	{
		GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& N, vec<L, T, Q> const& I, vec<L, T, Q> const& Nref)
		{
			return dot(Nref, I) < static_cast<T>(0) ? N : -N;
		}
	};

	template<length_t L, typename T, qualifier Q, bool Aligned>
	struct compute_reflect
	{
		GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& I, vec<L, T, Q> const& N)
		{
			return I - N * dot(N, I) * static_cast<T>(2);
		}
	};

	template<length_t L, typename T, qualifier Q, bool Aligned>
	struct compute_refract
	{
		GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& I, vec<L, T, Q> const& N, T eta)
		{
			T const dotValue(dot(N, I));
			T const k(static_cast<T>(1) - eta * eta * (static_cast<T>(1) - dotValue * dotValue));
			vec<L, T, Q> const Result =
                (k >= static_cast<T>(0)) ? (eta * I - (eta * dotValue + std::sqrt(k)) * N) : vec<L, T, Q>(0);
			return Result;
		}
	};
}//namespace detail

	// length
	template<typename genType>
	GLM_FUNC_QUALIFIER genType length(genType x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559, "'length' accepts only floating-point inputs");

		return abs(x);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER T length(vec<L, T, Q> const& v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'length' accepts only floating-point inputs");

		return detail::compute_length<L, T, Q, detail::is_aligned<Q>::value>::call(v);
	}

	// distance
	template<typename genType>
	GLM_FUNC_QUALIFIER genType distance(genType const& p0, genType const& p1)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559, "'distance' accepts only floating-point inputs");

		return length(p1 - p0);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER T distance(vec<L, T, Q> const& p0, vec<L, T, Q> const& p1)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'distance' accepts only floating-point inputs");

		return detail::compute_distance<L, T, Q, detail::is_aligned<Q>::value>::call(p0, p1);
	}

	// dot
	template<typename T>
	GLM_FUNC_QUALIFIER T dot(T x, T y)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'dot' accepts only floating-point inputs");

		return x * y;
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER T dot(vec<L, T, Q> const& x, vec<L, T, Q> const& y)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'dot' accepts only floating-point inputs");

		return detail::compute_dot<vec<L, T, Q>, T, detail::is_aligned<Q>::value>::call(x, y);
	}

	// cross
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<2, T, Q> cross(vec<2, T, Q> const& x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'cross' accepts only floating-point inputs");

		return detail::compute_cross<2, T, Q, detail::is_aligned<Q>::value>::call(x);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<3, T, Q> cross(vec<3, T, Q> const& x, vec<3, T, Q> const& y)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'cross' accepts only floating-point inputs");

		return detail::compute_cross<3, T, Q, detail::is_aligned<Q>::value>::call(x, y);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<4, T, Q> cross(vec<4, T, Q> const& x, vec<4, T, Q> const& y, vec<4, T, Q> const& z)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'cross' accepts only floating-point inputs");

		return detail::compute_cross<4, T, Q, detail::is_aligned<Q>::value>::call(x, y , z);
	}

	template<length_t N, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<N, N, T, Q> tensor_cross(vec<N, T, Q> const& x, vec<N, T, Q> const& y)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'tensor_cross' accepts only floating-point inputs");

		return detail::compute_cross_as_tensor<N, T, Q, detail::is_aligned<Q>::value>::call(x, y);
	}

/*
	// normalize
	template<typename genType>
	GLM_FUNC_QUALIFIER genType normalize(genType const& x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559, "'normalize' accepts only floating-point inputs");

		return x < genType(0) ? genType(-1) : genType(1);
	}
*/
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> normalize(vec<L, T, Q> const& x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");

		return detail::compute_normalize<L, T, Q, detail::is_aligned<Q>::value>::call(x);
	}

	// faceforward
	template<typename genType>
	GLM_FUNC_QUALIFIER genType faceforward(genType const& N, genType const& I, genType const& Nref)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559, "'faceforward' accepts only floating-point inputs");

		return dot(Nref, I) < static_cast<genType>(0) ? N : -N;
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> faceforward(vec<L, T, Q> const& N, vec<L, T, Q> const& I, vec<L, T, Q> const& Nref)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'faceforward' accepts only floating-point inputs");

		return detail::compute_faceforward<L, T, Q, detail::is_aligned<Q>::value>::call(N, I, Nref);
	}

	// reflect
	template<typename genType>
	GLM_FUNC_QUALIFIER genType reflect(genType const& I, genType const& N)
	{
		return I - N * dot(N, I) * genType(2);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> reflect(vec<L, T, Q> const& I, vec<L, T, Q> const& N)
	{
		return detail::compute_reflect<L, T, Q, detail::is_aligned<Q>::value>::call(I, N);
	}

	// refract
	template<typename genType>
	GLM_FUNC_QUALIFIER genType refract(genType const& I, genType const& N, genType eta)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559, "'refract' accepts only floating-point inputs");

		genType const dotValue(dot(N, I));
		genType const k(static_cast<genType>(1) - eta * eta * (static_cast<genType>(1) - dotValue * dotValue));
		return (eta * I - (eta * dotValue + sqrt(k)) * N) * static_cast<genType>(k >= static_cast<genType>(0));
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> refract(vec<L, T, Q> const& I, vec<L, T, Q> const& N, T eta)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'refract' accepts only floating-point inputs");

		return detail::compute_refract<L, T, Q, detail::is_aligned<Q>::value>::call(I, N, eta);
	}
}//namespace glm

#if GLM_CONFIG_SIMD == GLM_ENABLE
#	include "func_geometric_simd.inl"
#endif
