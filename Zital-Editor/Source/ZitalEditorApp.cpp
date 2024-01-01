#include <Zital.h>
#include <Zital/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include "EditorLayer.h"


namespace Zital
{

	class ZitalEditor : public Application
	{
	public:
		ZitalEditor()
			: Application("Zital Editor")
		{
			PushLayer(new EditorLayer());
		}

		~ZitalEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new ZitalEditor();
	}

}