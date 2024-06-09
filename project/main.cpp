#include "SDLApplication.h"

#include <stdio.h>

int main() {
    printf("Hello, World!\n");

	SDLApplication* app;
	SDLWindow* window;

	{
		int flags = 0;
		flags |= WINDOW_FLAG_HARDWARE;
		flags |= WINDOW_FLAG_COLOR_DEPTH_32_BIT;

		app = new SDLApplication();
		window = new SDLWindow(app, 800, 600, flags, "Window Title");

	}

	app->Exec();
	window->Close();

	delete app;
	delete window;
    return 0;
}
