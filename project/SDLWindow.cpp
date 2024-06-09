#include "SDLWindow.h"
#include "SDLApplication.h"
#include "OpenGL.h"

#ifdef WINDOWS

#include <windows.h>
#include <SDL_syswm.h>
#include <cstdio>

#elif defined(LINUX)

#include <cstdio>
#include <cstring>

#endif

SDLWindow::SDLWindow(Application* application, int width, int height, int flags, const char* title) {
    sdlRenderer = 0;
	sdlTexture = 0;
	context = 0;

	contextWidth = width;
	contextHeight = height;

	currentApplication = application;
	this->flags = flags;

	int sdlWindowFlags = 0;

	    if (flags & WINDOW_FLAG_FULLSCREEN) sdlWindowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (flags & WINDOW_FLAG_RESIZABLE) sdlWindowFlags |= SDL_WINDOW_RESIZABLE;
		if (flags & WINDOW_FLAG_BORDERLESS) sdlWindowFlags |= SDL_WINDOW_BORDERLESS;
		if (flags & WINDOW_FLAG_HIDDEN) sdlWindowFlags |= SDL_WINDOW_HIDDEN;
		if (flags & WINDOW_FLAG_MINIMIZED) sdlWindowFlags |= SDL_WINDOW_MINIMIZED;
		if (flags & WINDOW_FLAG_MAXIMIZED) sdlWindowFlags |= SDL_WINDOW_MAXIMIZED;

    #ifndef EMSCRIPTEN
    if (flags & WINDOW_FLAG_ALWAYS_ON_TOP) sdlWindowFlags |= SDL_WINDOW_ALWAYS_ON_TOP;
    #endif

	// TODO: Has not been tested
    #if defined(WINDOWS) && defined(NATIVE_TOOLKIT_SDL_ANGLE) && !defined(WINRT)
		OSVERSIONINFOEXW osvi = { sizeof (osvi), 0, 0, 0, 0, {0}, 0, 0 };
		DWORDLONG const dwlConditionMask = VerSetConditionMask (VerSetConditionMask (VerSetConditionMask (0, VER_MAJORVERSION, VER_GREATER_EQUAL), VER_MINORVERSION, VER_GREATER_EQUAL), VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
		osvi.dwMajorVersion = HIBYTE (_WIN32_WINNT_VISTA);
		osvi.dwMinorVersion = LOBYTE (_WIN32_WINNT_VISTA);
		osvi.wServicePackMajor = 0;

		if (VerifyVersionInfoW (&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) == FALSE) {
			flags &= ~WINDOW_FLAG_HARDWARE;
		}
    #endif

    #if !defined(EMSCRIPTEN)
    SDL_SetHint (SDL_HINT_ANDROID_TRAP_BACK_BUTTON, "0");
    SDL_SetHint (SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
    SDL_SetHint (SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
    SDL_SetHint (SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
    #endif

    if (flags & WINDOW_FLAG_HARDWARE) {
		sdlWindowFlags |= SDL_WINDOW_OPENGL;

		if (flags & WINDOW_FLAG_ALLOW_HIGHDPI) {
			sdlWindowFlags |= SDL_WINDOW_ALLOW_HIGHDPI;
		}

		#if defined(WINDOWS) && defined(NATIVE_TOOLKIT_SDL_ANGLE)
		SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_SetHint (SDL_HINT_VIDEO_HIT_D3DCOMPILER, "d3dcompiler_47.dll");
		#endif

		#if defined(IPHONE)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		#endif

		if(flags & WINDOW_FLAG_DEPTH_BUFFER) {
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32 - (flags & WINDOW_FLAG_STENCIL_BUFFER ? 8 : 0));
		}

		if(flags & WINDOW_FLAG_STENCIL_BUFFER) {
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		}

		if(flags & WINDOW_FLAG_HW_AA_HIRES) {
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		}else if(flags & WINDOW_FLAG_HW_AA) {
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
		}

		if(flags & WINDOW_FLAG_COLOR_DEPTH_32_BIT) {
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		}else {
		    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
		}

		sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdlWindowFlags);

		#if defined(IPHONE)
		if(sdlWindow && !SDL_GL_CreateContext(sdlWindow)) {
			SDL_DestroyWindow(sdlWindow);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

			sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdlWindowFlags);
		}
		#endif

		if(!sdlWindow) {
		    printf("Failed to create window: %s\n", SDL_GetError());
			return;
		}

		#if defined(WINDOWS) && defined(NATIVE_TOOLKIT_SDL_ANGLE)
		HINSTANCE instance = GetModuleHandle(0);
		HICON icon = LoadIcon(instance, MAKEINTRESOURCE(101));

		if(icon) {
			SDL_SysWMinfo info;
			SDL_VERSION(&info.version);

			if(SDL_GetWindowWMInfo(sdlWindow, &info)) {
				HWND hwnd = info.info.win.window;

				#ifdef _WIN64
				::SetClassLongPtr(hwnd, GCLP_HICON, reinterpret_cast<LONG_PTR>(icon));
				#else
				::SetClassLong(hwnd, GCL_HICON, reinterpret_cast<LONG>(icon));
				#endif
			}
		}
		#endif

		int sdlRendererFlags = 0;

		if(flags & WINDOW_FLAG_HARDWARE) {
			sdlRendererFlags |= SDL_RENDERER_ACCELERATED;

		    #ifdef EMSCRIPTEN
			sdlRendererFlags |= SDL_RENDERER_PRESENTVSYNC;
			#endif

			context = SDL_GL_CreateContext(sdlWindow);

			if(context && SDL_GL_MakeCurrent(sdlWindow, context) == 0) {
				if(flags & WINDOW_FLAG_VSYNC) {
					SDL_GL_SetSwapInterval(1);
				}else {
					SDL_GL_SetSwapInterval(0);
				}

				InitOpenGLExtensions();

				#ifndef GLES
				int version = 0;
				glGetIntegerv(GL_MAJOR_VERSION, &version);

				if(version == 0) {
				    float versionScan = 0;
					sscanf((const char*)glGetString(GL_VERSION), "%f", &versionScan);
					version = (int)versionScan;
				}

				if(version < 2 && !strstr((const char*)glGetString(GL_EXTENSIONS), "OpenGL ES")) {
				    SDL_GL_DeleteContext(context);
					context = 0;
				}

				#elif defined(IPHONE)
				glGetIntegerv(GL_FRAMEBUFFER_BINDING, &render.defaultFramebuffer);
				glGetIntegerv(GL_RENDERBUFFER_BINDING, &render.defaultRenderbuffer);
				#endif
			}else {
				SDL_GL_DeleteContext(context);
				context = 0;
			}
		}

		if(!context) {
		    sdlRendererFlags &= ~SDL_RENDERER_ACCELERATED;
			sdlRendererFlags &= SDL_RENDERER_PRESENTVSYNC;
		    sdlRendererFlags |= SDL_RENDERER_SOFTWARE;

			sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, sdlRendererFlags);
		}

		if(context || sdlRenderer) {
		    ((SDLApplication*)currentApplication)->RegisterWindow(this);
		}else {
		    printf("Failed to create OpenGL context: %s\n", SDL_GetError());
		}
	}
}

void SDLWindow::Close() {
    if(sdlWindow) {
	    SDL_DestroyWindow(sdlWindow);
		sdlWindow = 0;
	}
}

void SDLWindow::ContextFlip() {
    if(context && !sdlRenderer) {
	    SDL_GL_SwapWindow(sdlWindow);
	}else if(sdlRenderer) {
	    SDL_RenderPresent(sdlRenderer);
	}
}

SDLWindow::~SDLWindow() {
		if(sdlWindow) {
		    SDL_DestroyWindow(sdlWindow);
			sdlWindow = 0;
		}

		if(sdlRenderer) {
		    SDL_DestroyRenderer(sdlRenderer);
			sdlRenderer = 0;
		}else if(context) {
		    SDL_GL_DeleteContext(context);
			context = 0;
		}
}
