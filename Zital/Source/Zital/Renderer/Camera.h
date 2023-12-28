#pragma once

#include <glm/glm.hpp>

namespace Zital
{

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& _projection)
			: mProjection(_projection){}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return mProjection; }

	protected:
		glm::mat4 mProjection = glm::mat4(1.f);
	};

}