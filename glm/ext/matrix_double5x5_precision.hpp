/// @ref core
/// @file glm/ext/matrix_double5x5_precision.hpp

#pragma once
#include "../detail/type_mat5x5.hpp"

namespace glm
{
	/// @addtogroup core_matrix_precision
	/// @{

	/// 5 columns of 5 components matrix of double-precision floating-point numbers using low precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, double, lowp>		lowp_dmat5;

	/// 5 columns of 5 components matrix of double-precision floating-point numbers using medium precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, double, mediump>	mediump_dmat5;

	/// 5 columns of 5 components matrix of double-precision floating-point numbers using medium precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, double, highp>	highp_dmat5;

	/// 5 columns of 5 components matrix of double-precision floating-point numbers using low precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, double, lowp>		lowp_dmat5x5;

	/// 5 columns of 5 components matrix of double-precision floating-point numbers using medium precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, double, mediump>	mediump_dmat5x5;

	/// 5 columns of 5 components matrix of double-precision floating-point numbers using medium precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, double, highp>	highp_dmat5x5;

	/// @}
}//namespace glm
