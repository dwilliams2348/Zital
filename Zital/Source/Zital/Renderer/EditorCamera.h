#pragma once

#include "Zital/Renderer/Camera.h"
#include "Zital/Core/TimeStep.h"
#include "Zital/Events/Event.h"
#include "Zital/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Zital
{

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float _fov, float _aspectRatio, float _nearClip, float _farClip);

		void OnUpdate(Timestep _deltaTime);
		void OnEvent(Event& e);

		inline float GetDistance() const { return mDistance; }
		inline void SetDistance(float _distance) { mDistance = _distance; }

		inline void SetViewportSize(float _width, float _height) { mViewportWidth = _width; mViewportHeight = _height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
		glm::mat4 GetViewProjection() const { return mProjection * mViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return mPosition; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return mPitch; }
		float GetYaw() const { return mYaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& _delta);
		void MouseRotate(const glm::vec2& _delta);
		void MouseZoom(float _delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		float mFOV = 45.f, mAspectRatio = 1.778f, mNearClip = 0.1f, mFarClip = 1000.f;

		glm::mat4 mViewMatrix;
		glm::vec3 mPosition = { 0.f, 0.f, 0.f };
		glm::vec3 mFocalPoint = { 0.f, 0.f, 0.f };

		glm::vec2 mInitialMousePosition;

		float mDistance = 10.f;
		float mPitch = 0.f, mYaw = 0.f;

		float mViewportWidth = 1280.f, mViewportHeight = 720.f;
	};

}