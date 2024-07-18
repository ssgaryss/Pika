#pragma once
#include <glm/glm.hpp>

namespace Pika::Math
{
	
	bool decomposeTransform(const glm::mat4& vTransform, glm::vec3& vTranslation,
		glm::vec3& vRotation, glm::vec3& vScale);

}
