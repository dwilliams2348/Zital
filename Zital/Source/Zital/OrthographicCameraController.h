#pragma once

#include "Zital/Renderer/OrthographicCamera.h"
#include "Zital/Core/Timestep.h"

#include "Zital/Events/ApplicationEvent.h"
#include "Zital/Events/MouseEvent.h"

namespace Zital
{

	struct OrthoGraphicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float _aspectRatio, bool _rotation = false);

		void OnUpdate(Timestep _deltaTime);
		void OnEvent(Event& e);

		void OnResize(float _width, float _height);

		OrthographicCamera& GetCamera() { return mCamera; }
		const OrthographicCamera& GetCamera() const { return mCamera; }

		float GetZoomLevel() const { return mZoomLevel; }
		void SetZoomLevel(float _level) { mZoomLevel = _level; CalculateView(); }

		const OrthoGraphicCameraBounds& GetBounds() const { return mBounds; }

	private:
		void CalculateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float mAspectRatio;
		float mZoomLevel = 1.f;
		OrthoGraphicCameraBounds mBounds;
		OrthographicCamera mCamera;

		bool mRotation;

		glm::vec3 mCameraPosition = { 0.f, 0.f, 0.f };
		float mCameraRotation = 0.f;
		float mCameraTranslationSpeed = 2.5f, mCameraRotationSpeed = 90.f;
	};

}