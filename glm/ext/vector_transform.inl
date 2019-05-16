namespace glm
{
	template<length_t N, typename T, qualifier Q>
	GLM_FUNC_DECL vec<N, T, Q> rotate(vec<N, T, Q> const& t, vec<N, T, Q> const& b, T angle, vec<N, T, Q> const& v)
	{
		T const tProj = glm::dot(v, t);
		T const bProj = glm::dot(v, b);
		T const c = cos(angle);
		T const s = sin(angle);

		mat<2, 2, T, Q> Rotate;
		Rotate[0][0] = c;
		Rotate[0][1] = s;
		Rotate[1][0] = -s;
		Rotate[1][1] = c;

		vec<2, T, Q> const newTangentComponents = Rotate * vec<2, T, Q>(tProj, bProj);

		vec<N, T, Q> Result = v + (newTangentComponents[0] - tProj) * t + (newTangentComponents[1] - bProj) * b;

		return Result;
	}
}//namespace glm
