#pragma once

#include <glm/glm.hpp>

namespace Zital
{

	class Camera
	{
	public:
		Camera(const glm::mat4& _projection)
			: mProjection(_projection){}

		const glm::mat4& GetProjection() const { return mProjection; }

	private:
		glm::mat4 mProjection;
	};

}