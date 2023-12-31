#include "ZTpch.h"
#include "OrthographicCameraController.h"

#include "Zital/Core/Input.h"
#include "Zital/Core/KeyCodes.h"

namespace Zital
{
	OrthographicCameraController::OrthographicCameraController(float _aspectRatio, bool _rotation)
		: mAspectRatio(_aspectRatio), mRotation(_rotation), mBounds({-mAspectRatio * mZoomLevel, mAspectRatio* mZoomLevel, -mZoomLevel, mZoomLevel}), mCamera(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep _deltaTime)
	{
		ZT_PROFILE_FUNCTION();

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
		ZT_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ZT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(ZT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float _width, float _height)
	{
		mAspectRatio = _width / _height;
		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		mBounds = { -mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel };
		mCamera.SetProjection(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		ZT_PROFILE_FUNCTION();

		mZoomLevel -= e.GetYOffset() * 0.25f;
		mZoomLevel = std::max(mZoomLevel, 0.5f);

		CalculateView();

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		ZT_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}
}