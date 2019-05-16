/// @ref core
/// @file glm/ext/matrix_float5x5_precision.hpp

#pragma once
#include "../detail/type_mat5x5.hpp"

namespace glm
{
	/// @addtogroup core_matrix_precision
	/// @{

	/// 5 columns of 5 components matrix of single-precision floating-point numbers using low precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, float, lowp>		lowp_mat5;

	/// 5 columns of 5 components matrix of single-precision floating-point numbers using medium precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, float, mediump>	mediump_mat5;

	/// 5 columns of 5 components matrix of single-precision floating-point numbers using high precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, float, highp>		highp_mat5;

	/// 5 columns of 5 components matrix of single-precision floating-point numbers using low precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, float, lowp>		lowp_mat5x5;

	/// 5 columns of 5 components matrix of single-precision floating-point numbers using medium precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, float, mediump>	mediump_mat5x5;

	/// 5 columns of 5 components matrix of single-precision floating-point numbers using high precision arithmetic in term of ULPs.
	///
	typedef mat<5, 5, float, highp>		highp_mat5x5;

	/// @}
}//namespace glm
