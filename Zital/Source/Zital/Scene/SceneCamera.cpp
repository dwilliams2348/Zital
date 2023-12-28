#include "ZTpch.h"
#include "Zital/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Zital
{

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float _size, float _nearClip, float _farClip)
	{
		mOrthographicSize = _size;
		mOrthographicNear = _nearClip;
		mOrthographicFar = _farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t _width, uint32_t _height)
	{
		mAspectRatio = (float)_width / (float)_height;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -mOrthographicSize * mAspectRatio * 0.5f;
		float orthoRight = mOrthographicSize * mAspectRatio * 0.5f;
		float orthoBottom = -mOrthographicSize * 0.5f;
		float orthoTop = mOrthographicSize * 0.5f;

		mProjection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, mOrthographicNear, mOrthographicFar);
	}

}