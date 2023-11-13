#include "ZTpch.h"
#include "Application.h"

#include "Zital/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Zital
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::sInstance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType _type)
	{
		switch (_type)
		{
			case Zital::ShaderDataType::Float:		return GL_FLOAT;
			case Zital::ShaderDataType::Float2:		return GL_FLOAT;
			case Zital::ShaderDataType::Float3:		return GL_FLOAT;
			case Zital::ShaderDataType::Float4:		return GL_FLOAT;
			case Zital::ShaderDataType::Mat3:		return GL_FLOAT;
			case Zital::ShaderDataType::Mat4:		return GL_FLOAT;
			case Zital::ShaderDataType::Int:		return GL_INT;
			case Zital::ShaderDataType::Int2:		return GL_INT;
			case Zital::ShaderDataType::Int3:		return GL_INT;
			case Zital::ShaderDataType::Int4:		return GL_INT;
			case Zital::ShaderDataType::Bool:		return GL_BOOL;
		}

		ZT_CORE_ASSERT(false, "Unknown shader data type.");
		return 0;
	}

	Application::Application()
	{
		ZT_CORE_ASSERT(!sInstance, "Application already exists.");
		sInstance = this;

		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);

		glGenVertexArrays(1, &mVertexArray);
		glBindVertexArray(mVertexArray);

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.f, 0.8f, 0.2f, 0.8f, 1.f,
			0.5f, -0.5f, 0.f, 0.2f, 0.2f, 0.8f, 1.f,
			0.f, 0.5f, 0.f, 0.8f, 0.8f, 0.2f, 1.f
		};

		mVertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			BufferLayout layout =
			{
				{ShaderDataType::Float3, "aPosition"},
				{ShaderDataType::Float4, "aColor"}
			};

			mVertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = mVertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride() , 
				(const void*)element.Offset);
			index++;
		}
		

		uint32_t indices[3] = { 0, 1, 2 };
		mIndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

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
			glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			mShader->Bind();
			glBindVertexArray(mVertexArray);
			glDrawElements(GL_TRIANGLES, mIndexBuffer->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

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