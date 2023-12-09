#include "ZTpch.h"
#include "OrthographicCameraController.h"

#include "Zital/Input.h"
#include "Zital/KeyCodes.h"

namespace Zital
{
	OrthographicCameraController::OrthographicCameraController(float _aspectRatio, bool _rotation)
		: mAspectRatio(_aspectRatio), mRotation(_rotation), mCamera(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep _deltaTime)
	{
		//camera movement + rotation
		if (Input::IsKeyPressed(ZT_KEY_A))
			mCameraPosition.x -= mCameraTranslationSpeed * _deltaTime;
		else if (Input::IsKeyPressed(ZT_KEY_D))
			mCameraPosition.x += mCameraTranslationSpeed * _deltaTime;

		if (Input::IsKeyPressed(ZT_KEY_W))
			mCameraPosition.y += mCameraTranslationSpeed * _deltaTime;
		else if (Input::IsKeyPressed(ZT_KEY_S))
			mCameraPosition.y -= mCameraTranslationSpeed * _deltaTime;

		if (mRotation)
		{
			if (Input::IsKeyPressed(ZT_KEY_Q))
				mCameraRotation += mCameraRotationSpeed * _deltaTime;
			else if (Input::IsKeyPressed(ZT_KEY_E))
				mCameraRotation -= mCameraRotationSpeed * _deltaTime;

			mCamera.SetRotation(mCameraRotation);
		}

		mCamera.SetPosition(mCameraPosition);

		mCameraTranslationSpeed = mZoomLevel * 1.5f;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ZT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(ZT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		mZoomLevel -= e.GetYOffset() * 0.25f;
		mZoomLevel = std::max(mZoomLevel, 0.5f);

		mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		mAspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);

		return false;
	}
}