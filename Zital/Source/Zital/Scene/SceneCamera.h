#pragma once

#include "Zital/Renderer/Camera.h"

namespace Zital
{

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float _size, float _nearClip, float _farClip);

		void SetViewportSize(uint32_t _width, uint32_t _height);

		float GetOrthographicSize() const { return mOrthographicSize; }
		void SetOrthographicSize(float _size) { mOrthographicSize = _size; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		float mOrthographicSize = 10.f;
		float mOrthographicNear = -1.f, mOrthographicFar = 1.f;

		float mAspectRatio = 1.f;
	};

}