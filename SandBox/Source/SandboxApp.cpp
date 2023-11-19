#include <Zital.h>

#include "imgui/imgui.h"

class ExampleLayer : public Zital::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), mCamera(-1.6f, 1.6f, -0.9f, 0.9f), mCameraPosition({0.f, 0.f, 0.f})
	{
		mVertexArray.reset(Zital::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.f, 0.8f, 0.2f, 0.8f, 1.f,
			0.5f, -0.5f, 0.f, 0.2f, 0.2f, 0.8f, 1.f,
			0.f, 0.5f, 0.f, 0.8f, 0.8f, 0.2f, 1.f
		};

		std::shared_ptr<Zital::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Zital::VertexBuffer::Create(vertices, sizeof(vertices)));

		Zital::BufferLayout layout =
		{
			{Zital::ShaderDataType::Float3, "aPosition"},
			{Zital::ShaderDataType::Float4, "aColor"}
		};
		vertexBuffer->SetLayout(layout);

		mVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Zital::IndexBuffer> indexBuffer;
		indexBuffer.reset(Zital::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		mVertexArray->SetIndexBuffer(indexBuffer);

		//rendering square to screen with mutliple arrays
		mSquareVA.reset(Zital::VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.75f, -0.75f, 0.f,
			0.75f, -0.75f, 0.f,
			0.75f, 0.75f, 0.f,
			-0.75f, 0.75f, 0.f
		};

		std::shared_ptr<Zital::VertexBuffer> SquareVB;
		SquareVB.reset(Zital::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		SquareVB->SetLayout({ {Zital::ShaderDataType::Float3, "aPosition"} });

		mSquareVA->AddVertexBuffer(SquareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Zital::IndexBuffer> squareIB;
		squareIB.reset(Zital::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		mSquareVA->SetIndexBuffer(squareIB);

		//the "()" lets you write strings across multiple lines without having to put double quotes on each line to start and end the string and without
		//using the \n character.
		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec4 aColor;

			uniform mat4 uViewProjection;

			out vec3 vPosition;
			out vec4 vColor;

			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = uViewProjection * vec4(aPosition, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 vPosition;
			in vec4 vColor;

			void main()
			{
				color = vec4(vPosition + 0.5, 1.0);
				color = vColor;
			}
		)";

		mShader.reset(new Zital::Shader(vertexSource, fragmentSource));

		std::string blueVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 aPosition;

			uniform mat4 uViewProjection;

			out vec3 vPosition;

			void main()
			{
				vPosition = aPosition;
				gl_Position = uViewProjection * vec4(aPosition, 1.0);
			}
		)";

		std::string blueFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 vPosition;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1);
			}
		)";

		mBlueSquareShader.reset(new Zital::Shader(blueVertexSource, blueFragmentSource));
	}


	void OnUpdate(Zital::TimeStep _deltaTime) override
	{
		//ZT_TRACE("Delta time: {0}s ({1}ms)", _deltaTime.GetSeconds(), _deltaTime.GetMilliseconds());

		if (Zital::Input::IsKeyPressed(ZT_KEY_LEFT))
			mCameraPosition.x -= mCameraMoveSpeed * _deltaTime;
		else if (Zital::Input::IsKeyPressed(ZT_KEY_RIGHT))
			mCameraPosition.x += mCameraMoveSpeed * _deltaTime;

		if (Zital::Input::IsKeyPressed(ZT_KEY_UP))
			mCameraPosition.y += mCameraMoveSpeed * _deltaTime;
		else if (Zital::Input::IsKeyPressed(ZT_KEY_DOWN))
			mCameraPosition.y -= mCameraMoveSpeed * _deltaTime;

		if (Zital::Input::IsKeyPressed(ZT_KEY_A))
			mCameraRotation += mCameraRotateSpeed * _deltaTime;
		else if (Zital::Input::IsKeyPressed(ZT_KEY_D))
			mCameraRotation -= mCameraRotateSpeed * _deltaTime;


		Zital::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Zital::RenderCommand::Clear();

		mCamera.SetPosition(mCameraPosition);
		mCamera.SetRotation(mCameraRotation);

		Zital::Renderer::BeginScene(mCamera);

		//add meshes/geometry
		Zital::Renderer::Submit(mBlueSquareShader, mSquareVA);
		Zital::Renderer::Submit(mShader, mVertexArray);

		Zital::Renderer::EndScene();

		//flushing render queue
		//Renderer::Flush();
	}

	virtual void OnImGuiRender()
	{
		
	}

	void OnEvent(Zital::Event& _event) override
	{
	}

private:
	std::shared_ptr<Zital::Shader> mShader;
	std::shared_ptr<Zital::VertexArray> mVertexArray;

	std::shared_ptr<Zital::Shader> mBlueSquareShader;
	std::shared_ptr<Zital::VertexArray> mSquareVA;

	Zital::OrthographicCamera mCamera;
	glm::vec3 mCameraPosition;
	float mCameraMoveSpeed = 2.5f;

	float mCameraRotation = 0.f;
	float mCameraRotateSpeed = 90.f;
};

class Sandbox : public Zital::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Zital::Application* Zital::CreateApplication()
{
	return new Sandbox();
}