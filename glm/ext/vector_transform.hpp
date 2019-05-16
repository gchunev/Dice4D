/// @ref ext_vector_transform
/// @file glm/ext/vector_transform.hpp
///
/// @defgroup ext_vector_transform GLM_EXT_vector_transform
/// @ingroup ext
///
/// Defines functions that generate common transformations using vectors.
///
/// Include <glm/ext/vector_transform.hpp> to use the features of this extension.
///
/// @see ext_matrix_projection
/// @see ext_matrix_clip_space

#pragma once

// Dependencies
#include "../gtc/constants.hpp"
#include "../geometric.hpp"
#include "../trigonometric.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_vector_transform extension included")
#endif

namespace glm
{
	/// @addtogroup ext_vector_transform
	/// @{
	
	/// Performs a rotation in the plane spanned by the two given unit vectors.
	/// For positive angles, the rotation takes the first vector towards the second.
	/// The two vectors should be of unit length and not be collinear.
	///
	/// @param t the first tangent vector to the rotation plane
	/// @param b the second tangent vector to the rotation plane
	/// @param angle Rotation angle expressed in radians.
	/// @param v the vector to be rotated.
	///
	/// @tparam T A floating-point scalar type
	/// @tparam Q A value from qualifier enum
	///
	template<length_t N, typename T, qualifier Q>
	GLM_FUNC_DECL vec<N, T, Q> rotate(
		vec<N, T, Q> const& t, vec<N, T, Q> const& b, T angle, vec<N, T, Q> const& v);

	/// @}
}//namespace glm

#include "vector_transform.inl"
