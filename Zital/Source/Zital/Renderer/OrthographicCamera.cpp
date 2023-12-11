#include "ZTpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Zital
{

	OrthographicCamera::OrthographicCamera(float _left, float _right, float _bottom, float _top)
		: mProjectionMatrix(glm::ortho(_left, _right, _bottom, _top, -1.f, 1.f)), mViewMatrix(1.f)
	{
		ZT_PROFILE_FUNCTION();

		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void OrthographicCamera::SetProjection(float _left, float _right, float _bottom, float _top)
	{
		ZT_PROFILE_FUNCTION();

		mProjectionMatrix = glm::ortho(_left, _right, _bottom, _top, -1.f, 1.f);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		ZT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.f), mPosition)
			* glm::rotate(glm::mat4(1.f), glm::radians(mRotation), glm::vec3(0, 0, 1));

		mViewMatrix = glm::inverse(transform);

		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

}