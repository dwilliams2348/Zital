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
		ZitalEditor(ApplicationCommandLineArgs _args)
			: Application("Zital Editor", _args)
		{
			PushLayer(new EditorLayer());
		}

		~ZitalEditor()
		{

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs _args)
	{
		return new ZitalEditor(_args);
	}

}