#include <Zital.h>
#include <Zital/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"


class ExampleLayer : public Zital::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), mCameraController(1280.f / 720.f)
	{
		mVertexArray = Zital::VertexArray::Create();

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.f, 0.8f, 0.2f, 0.8f, 1.f,
			0.5f, -0.5f, 0.f, 0.2f, 0.2f, 0.8f, 1.f,
			0.f, 0.5f, 0.f, 0.8f, 0.8f, 0.2f, 1.f
		};

		Zital::Ref<Zital::VertexBuffer> vertexBuffer;
		vertexBuffer = Zital::VertexBuffer::Create(vertices, sizeof(vertices));

		Zital::BufferLayout layout =
		{
			{Zital::ShaderDataType::Float3, "aPosition"},
			{Zital::ShaderDataType::Float4, "aColor"}
		};
		vertexBuffer->SetLayout(layout);

		mVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Zital::Ref<Zital::IndexBuffer> indexBuffer;
		indexBuffer = Zital::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		mVertexArray->SetIndexBuffer(indexBuffer);

		//rendering square to screen with mutliple arrays
		mSquareVA = Zital::VertexArray::Create();

		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.f, 0.f, 0.f,
			 0.5f, -0.5f, 0.f, 1.f, 0.f,
			 0.5f,  0.5f, 0.f, 1.f, 1.f,
			-0.5f,  0.5f, 0.f, 0.f, 1.f
		};

		Zital::Ref<Zital::VertexBuffer> SquareVB;
		SquareVB = Zital::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		SquareVB->SetLayout({
			{Zital::ShaderDataType::Float3, "aPosition"},
			{Zital::ShaderDataType::Float2, "aTexCoord"} });

		mSquareVA->AddVertexBuffer(SquareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Zital::Ref<Zital::IndexBuffer> squareIB;
		squareIB = Zital::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		mSquareVA->SetIndexBuffer(squareIB);

		//the "()" lets you write strings across multiple lines without having to put double quotes on each line to start and end the string and without
		//using the \n character.
		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec4 aColor;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;

			out vec3 vPosition;
			out vec4 vColor;

			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
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

		mShader = Zital::Shader::Create("VertexPosColor", vertexSource, fragmentSource);

		std::string flatColorVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 aPosition;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;

			out vec3 vPosition;

			void main()
			{
				vPosition = aPosition;
				gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
			}
		)";

		std::string flatColorFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 vPosition;

			uniform vec3 uColor;

			void main()
			{
				color = vec4(uColor, 1.0);
			}
		)";

		mFlatColorShader = Zital::Shader::Create("FlatColor", flatColorVertexSource, flatColorFragmentSource);

		auto textureShader = mShaderLibrary.Load("Assets/Shaders/Texture.glsl");

		mTexture = Zital::Texture2D::Create("Assets/Textures/checkerboard.png");
		mTransparentTexture = Zital::Texture2D::Create("Assets/Textures/transparentImg.png");

		std::dynamic_pointer_cast<Zital::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Zital::OpenGLShader>(textureShader)->UploadUniformInt("uTexture", 0);
	}


	void OnUpdate(Zital::Timestep _deltaTime) override
	{
		//Update
		mCameraController.OnUpdate(_deltaTime);

		//Render
		Zital::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Zital::RenderCommand::Clear();

		Zital::Renderer::BeginScene(mCameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Zital::OpenGLShader>(mFlatColorShader)->Bind();
		std::dynamic_pointer_cast<Zital::OpenGLShader>(mFlatColorShader)->UploadUniformFloat3("uColor", mSquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.f);
				glm::mat4 transform = glm::translate(glm::mat4(1.f), pos) * scale;
				Zital::Renderer::Submit(mFlatColorShader, mSquareVA, transform);
			}
		}
		
		auto textureShader = mShaderLibrary.Get("Texture");

		mTexture->Bind();
		Zital::Renderer::Submit(textureShader, mSquareVA, glm::scale(glm::mat4(1.f), glm::vec3(1.5f)));
		mTransparentTexture->Bind();
		Zital::Renderer::Submit(textureShader, mSquareVA, glm::scale(glm::mat4(1.f), glm::vec3(1.5f)));

		//triangle
		//Zital::Renderer::Submit(mShader, mVertexArray);

		Zital::Renderer::EndScene();

		//flushing render queue
		//Renderer::Flush();
	}

	virtual void OnImGuiRender()
	{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(mSquareColor));
	ImGui::End();
	}

	void OnEvent(Zital::Event& _event) override
	{
		mCameraController.OnEvent(_event);
	}

private:
	Zital::ShaderLibrary mShaderLibrary;
	Zital::Ref<Zital::Shader> mShader;
	Zital::Ref<Zital::VertexArray> mVertexArray;

	Zital::Ref<Zital::Shader> mFlatColorShader;
	Zital::Ref<Zital::VertexArray> mSquareVA;

	Zital::Ref<Zital::Texture2D> mTexture, mTransparentTexture;

	Zital::OrthographicCameraController mCameraController;

	glm::vec3 mSquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Zital::Application
{
public:
	Sandbox()
	{
		this->GetWindow().SetVSync(false);
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		
	}

	~Sandbox()
	{

	}
};

Zital::Application* Zital::CreateApplication()
{
	return new Sandbox();
}