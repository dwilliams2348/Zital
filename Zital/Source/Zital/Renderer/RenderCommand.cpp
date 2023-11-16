#include "ZTpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Zital
{
	RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;
}