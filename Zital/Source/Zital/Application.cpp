#include "ZTpch.h"
#include "Application.h"

#include "Zital/Log.h"

#include "Zital/Renderer/Renderer.h"

#include "Input.h"

namespace Zital
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::sInstance = nullptr;

	Application::Application()
	{
		ZT_CORE_ASSERT(!sInstance, "Application already exists.");
		sInstance = this;

		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);

		mVertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.f, 0.8f, 0.2f, 0.8f, 1.f,
			0.5f, -0.5f, 0.f, 0.2f, 0.2f, 0.8f, 1.f,
			0.f, 0.5f, 0.f, 0.8f, 0.8f, 0.2f, 1.f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		BufferLayout layout =
		{
			{ShaderDataType::Float3, "aPosition"},
			{ShaderDataType::Float4, "aColor"}
		};
		vertexBuffer->SetLayout(layout);

		mVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		mVertexArray->SetIndexBuffer(indexBuffer);

		//rendering square to screen with mutliple arrays
		mSquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.75f, -0.75f, 0.f,
			0.75f, -0.75f, 0.f,
			0.75f, 0.75f, 0.f,
			-0.75f, 0.75f, 0.f
		};

		std::shared_ptr<VertexBuffer> SquareVB;
		SquareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		
		SquareVB->SetLayout({ {ShaderDataType::Float3, "aPosition"} });

		mSquareVA->AddVertexBuffer(SquareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		mSquareVA->SetIndexBuffer(squareIB);

		//the "()" lets you write strings across multiple lines without having to put double quotes on each line to start and end the string and without
		//using the \n character.
		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec4 aColor;

			out vec3 vPosition;
			out vec4 vColor;

			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = vec4(aPosition, 1.0);
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

		mShader.reset(new Shader(vertexSource, fragmentSource));

		std::string blueVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 aPosition;

			out vec3 vPosition;

			void main()
			{
				vPosition = aPosition;
				gl_Position = vec4(aPosition, 1.0);
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

		mBlueSquareShader.reset(new Shader(blueVertexSource, blueFragmentSource));
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* _layer)
	{
		mLayerStack.PushLayer(_layer);
		_layer->OnAttach();
	}

	void Application::PushOverlay(Layer* _overlay)
	{
		mLayerStack.PushOverlay(_overlay);
		_overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));

		for (auto it = mLayerStack.end(); it != mLayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (mRunning)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			RenderCommand::Clear();

			Renderer::BeginScene();

			//add meshes/geometry
			mBlueSquareShader->Bind();
			Renderer::Submit(mSquareVA);

			mShader->Bind();
			Renderer::Submit(mVertexArray);

			Renderer::EndScene();

			//flushing render queue
			//Renderer::Flush();

			for (Layer* layer : mLayerStack)
				layer->OnUpdate();

			mImGuiLayer->Begin();
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->End();

			mWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		mRunning = false;

		return true;
	}

}