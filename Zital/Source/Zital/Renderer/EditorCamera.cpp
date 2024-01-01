#include "ZTpch.h"
#include "Zital/Renderer/EditorCamera.h"

#include "Zital/Core/Input.h"
#include "Zital/Core/KeyCodes.h"
#include "Zital/Core/MouseButtonCodes.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPIREMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Zital
{

	EditorCamera::EditorCamera(float _fov, float _aspectRatio, float _nearClip, float _farClip)
		: mFOV(_fov), mAspectRatio(_aspectRatio), mNearClip(_nearClip), mFarClip(_farClip)
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep _deltaTime)
	{
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - mInitialMousePosition) * 0.003f;
			mInitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ZT_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.f, 1.f, 0.f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.f, 0.f, 0.f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.f, 0.f, -1.f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-mPitch, -mYaw, 0.f));
	}

	void EditorCamera::UpdateProjection()
	{
		mAspectRatio = mViewportWidth / mViewportHeight;
		mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearClip, mFarClip);
	}

	void EditorCamera::UpdateView()
	{
		//mYaw = mPitch = 0.f //lock cameras rotation
		mPosition = CalculatePosition();

		glm::quat orientation = GetOrientation();
		mViewMatrix = glm::translate(glm::mat4(1.f), mPosition) * glm::toMat4(orientation);
		mViewMatrix = glm::inverse(mViewMatrix);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();

		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& _delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		mFocalPoint += -GetRightDirection() * _delta.x * xSpeed * mDistance;
		mFocalPoint += GetUpDirection() * _delta.y * ySpeed * mDistance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& _delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.f : 1.f;
		mYaw += yawSign * _delta.x * RotationSpeed();
		mPitch += _delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float _delta)
	{
		mDistance -= _delta * ZoomSpeed();
		if (mDistance < 1.f)
		{
			mFocalPoint += GetForwardDirection();
			mDistance = 1.f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return mFocalPoint - GetForwardDirection() * mDistance;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(mViewportWidth / 1000.f, 2.4f);
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(mViewportHeight / 1000.f, 2.4f);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = mDistance * 0.2f;
		distance = std::max(distance, 0.f);
		float speed = distance * distance;
		speed = std::min(speed, 100.f);

		return speed;
	}

}