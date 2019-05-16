/// @ref core
/// @file glm/ext/vector_bool5_precision.hpp

#pragma once
#include "../detail/type_vec5.hpp"

namespace glm
{
	/// @addtogroup core_vector_precision
	/// @{

	/// 5 components vector of high qualifier bool numbers.
	///
	typedef vec<5, bool, highp>		highp_bvec5;

	/// 5 components vector of medium qualifier bool numbers.
	///
	typedef vec<5, bool, mediump>	mediump_bvec5;

	/// 5 components vector of low qualifier bool numbers.
	///
	typedef vec<5, bool, lowp>		lowp_bvec5;

	/// @}
}//namespace glm
