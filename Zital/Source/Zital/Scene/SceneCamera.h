#pragma once

#include "Zital/Renderer/Camera.h"

namespace Zital
{

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetPerspective(float _verticalFov, float _nearClip, float _farClip);

		void SetOrthographic(float _size, float _nearClip, float _farClip);

		void SetViewportSize(uint32_t _width, uint32_t _height);

		float GetPerspectiveVerticalFOV() const { return mPerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float _verticalFov) { mPerspectiveFOV = _verticalFov; }
		float GetPerspectiveNearClip() const { return mPerspectiveNear; }
		void SetPerspectiveNearClip(float _near) { mPerspectiveNear = _near; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return mPerspectiveFar; }
		void SetPerspectiveFarClip(float _far) { mPerspectiveFar = _far; RecalculateProjection(); }

		float GetOrthographicSize() const { return mOrthographicSize; }
		void SetOrthographicSize(float _size) { mOrthographicSize = _size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return mOrthographicNear; }
		void SetOrthographicNearClip(float _near) { mOrthographicNear = _near; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return mOrthographicFar; }
		void SetOrthographicFarClip(float _far) { mOrthographicFar = _far; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return mProjectionType; }
		void SetProjectionType(ProjectionType _projection) { mProjectionType = _projection; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType mProjectionType = ProjectionType::Orthographic;

		float mPerspectiveFOV = glm::radians(45.f);
		float mPerspectiveNear = 0.01f, mPerspectiveFar = 10000.f;

		float mOrthographicSize = 10.f;
		float mOrthographicNear = -1.f, mOrthographicFar = 1.f;

		float mAspectRatio = 1.f;
	};

}