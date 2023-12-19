#include "ZTpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Zital
{
	Scope<RendererAPI> RenderCommand::sRendererAPI = CreateScope<OpenGLRendererAPI>();
}