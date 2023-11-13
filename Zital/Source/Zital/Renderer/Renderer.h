#pragma once


namespace Zital
{

	enum class RendererAPI
	{
		None = 0, //headless or when rendering not needed
		OpenGL 
	};


	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return sRendererAPI; }
	private:
		static RendererAPI sRendererAPI;
	};

}