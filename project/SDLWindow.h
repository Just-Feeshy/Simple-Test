#pragma once

#include <SDL.h>

#include "ui/Window.h"

class SDLWindow : public Window {
		public:

			SDLWindow (Application* application, int width, int height, int flags, const char* title);
			~SDLWindow ();

			virtual void Close ();
			virtual void ContextFlip ();

			SDL_Renderer* sdlRenderer;
			SDL_Texture* sdlTexture;
			SDL_Window* sdlWindow;

		private:

#ifdef IPHONE
				struct Render {
					GLint defaultFramebuffer;
					GLint defaultRenderbuffer;
				};

				Render render;
#endif
			SDL_GLContext context;
			int contextHeight;
			int contextWidth;
};
