#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace fs {

	namespace math
	{
		/// @brief 分解变换矩阵.
		/// @param TransformMat 要分解的矩阵.
		/// @param ToTranslation 分解出的平移
		/// @param ToRotation 分解出的旋转
		/// @param ToScale 分解出的缩放
		/// @return 分解是否成功
		bool DecomposeTransform(const glm::mat4& in_transformMat, glm::vec3& ToTranslation, glm::vec3& ToRotation, glm::vec3& ToScale);
	}
}