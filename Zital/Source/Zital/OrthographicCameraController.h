#pragma once

#include "Zital/Renderer/OrthographicCamera.h"
#include "Zital/Core/Timestep.h"

#include "Zital/Events/ApplicationEvent.h"
#include "Zital/Events/MouseEvent.h"

namespace Zital
{

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float _aspectRatio, bool _rotation = false);

		void OnUpdate(Timestep _deltaTime);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return mCamera; }
		const OrthographicCamera& GetCamera() const { return mCamera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float mAspectRatio;
		float mZoomLevel = 1.f;
		OrthographicCamera mCamera;

		bool mRotation;

		glm::vec3 mCameraPosition = { 0.f, 0.f, 0.f };
		float mCameraRotation = 0.f;
		float mCameraTranslationSpeed = 2.5f, mCameraRotationSpeed = 90.f;
	};

}