#pragma once

#include <glm/glm.hpp>

namespace Zital
{

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float _left, float _right, float _bottom, float _top);
		void SetProjection(float _left, float _right, float _bottom, float _top);

		const glm::vec3& GetPosition() const { return mPosition; }
		void SetPosition(const glm::vec3& _position) { mPosition = _position; RecalculateViewMatrix(); }

		const float& GetRotation() const { return mRotation; }
		void SetRotation(const float& _rotation) { mRotation = _rotation; RecalculateViewMatrix(); }


		const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return mViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewProjectionMatrix;

		glm::vec3 mPosition = { 0.f, 0.f, 0.f };
		float mRotation = 0.f;
	};

}