#pragma once

#include <glm/glm.hpp>

namespace Zital::Math
{

		bool DecomposeTransform(const glm::mat4& _transform, glm::vec3& _outTranslation, glm::vec3& _outRotation, glm::vec3& _outScale);

}