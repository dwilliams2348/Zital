#include "ZTpch.h"
#include "Zital/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Zital
{

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float _verticalFov, float _nearClip, float _farClip)
	{
		mProjectionType = ProjectionType::Perspective;
		mPerspectiveFOV = _verticalFov;
		mPerspectiveNear = _nearClip;
		mPerspectiveFar = _farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float _size, float _nearClip, float _farClip)
	{
		mProjectionType = ProjectionType::Orthographic;
		mOrthographicSize = _size;
		mOrthographicNear = _nearClip;
		mOrthographicFar = _farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t _width, uint32_t _height)
	{
		ZT_CORE_ASSERT(_width > 0 && _height > 0, "Invalid viewport width and height");

		mAspectRatio = (float)_width / (float)_height;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		switch (mProjectionType)
		{
		case ProjectionType::Perspective:
			mProjection = glm::perspective(mPerspectiveFOV, mAspectRatio, mPerspectiveNear, mPerspectiveFar);

			break;
		case ProjectionType::Orthographic:
			float orthoLeft = -mOrthographicSize * mAspectRatio * 0.5f;
			float orthoRight = mOrthographicSize * mAspectRatio * 0.5f;
			float orthoBottom = -mOrthographicSize * 0.5f;
			float orthoTop = mOrthographicSize * 0.5f;

			mProjection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, mOrthographicNear, mOrthographicFar);

			break;
		}
	}

}