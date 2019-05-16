/// @ref core
/// @file glm/ext/vector_double5_precision.hpp

#pragma once
#include "../detail/setup.hpp"
#include "../detail/type_vec5.hpp"

namespace glm
{
	/// @addtogroup core_vector_precision
	/// @{

	/// 5 components vector of high double-qualifier floating-point numbers.
	/// There is no guarantee on the actual qualifier.
	///
	typedef vec<5, double, highp>		highp_dvec5;

	/// 5 components vector of medium double-qualifier floating-point numbers.
	/// There is no guarantee on the actual qualifier.
	///
	typedef vec<5, double, mediump>		mediump_dvec5;

	/// 5 components vector of low double-qualifier floating-point numbers.
	/// There is no guarantee on the actual qualifier.
	///
	typedef vec<5, double, lowp>		lowp_dvec5;

	/// @}
}//namespace glm
