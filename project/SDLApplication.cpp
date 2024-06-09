#include "SDLApplication.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

SDLApplication* SDLApplication::currentApplication = 0;

SDLApplication::SDLApplication() {
    uint32_t initFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER;

    #ifdef HAS_AUDIO
	initFlags |= SDL_INIT_AUDIO;
    #endif

	if (SDL_Init(initFlags) != 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return;
	}

	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    framePeriod = 1000.0 / 60.0;

    currentUpdate = 0;
    lastUpdate = 0;
    nextUpdate = 0;

    SDL_EventState (SDL_DROPFILE, SDL_ENABLE);
}

int SDLApplication::Exec() {
    Init();

    #ifdef EMSCRIPTEN // This might be important for testing purposes for ANGLE
    emscripten_cancel_main_loop();
	emscripten_set_main_loop(UpdateFrame, 0, 0);
	emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
    #endif

    #if defined(IPHONE) || defined(EMSCRIPTEN)
	return 0;
    #else

	while(active) {
		Update();
	}

	return Quit();
    #endif
}

static SDL_TimerID timerID = 0;
bool timerActive = false;
bool firstTime = true;
bool inBackground = false;

uint32_t OnTimer(uint32_t interval, void*) {
    SDL_Event event;
    SDL_UserEvent userevent;
	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = 0;
	userevent.data2 = 0;
	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return 0;
}

bool SDLApplication::Update() {
    SDL_Event event;
	event.type = -1;

    #if !defined(IPHONE) && !defined(EMSCRIPTEN)
	if(active && (firstTime || SDL_WaitEvent(&event))) {
		firstTime = false;

		HandleEvent(&event);
		event.type = -1;
		if(!active) {
		    return false;
		}
    #endif

	while(SDL_PollEvent(&event)) {
		HandleEvent(&event);
		event.type = -1;

		if(!active) {
			return false;
		}
	}

	currentUpdate = SDL_GetTicks();

    #if defined(IPHONE) || defined(EMSCRIPTEN)
	if(currentUpdate >= nextUpdate) {
		event.type = SDL_USEREVENT;
		HandleEvent(&event);
		event.type = -1;
	}
    #else
		if(currentUpdate >= nextUpdate) {
		    if(timerActive) SDL_RemoveTimer(timerID);
		    OnTimer(0, 0);
		}else if(!timerActive) {
		    timerID = SDL_AddTimer(nextUpdate - currentUpdate, OnTimer, 0);
		    timerActive = true;
		}
	}
    #endif

    return active;
}

void SDLApplication::UpdateFrame() {
    currentApplication->Update();
}

void SDLApplication::UpdateFrame(void*) {
    currentApplication->Update();
}

void SDLApplication::HandleEvent(SDL_Event* event) {
    switch(event->type) {
		case SDL_USEREVENT:
		    if(!inBackground) {
				currentUpdate = SDL_GetTicks();
				lastUpdate = currentUpdate;
				nextUpdate = framePeriod;

				while(nextUpdate <= currentUpdate) {
					nextUpdate += framePeriod;
				}

				RenderEvent::Dispatch(&renderEvent);
		    }

		    break;
		case SDL_WINDOWEVENT: // TODO: Implement this
		    break;
		case SDL_QUIT:
		    active = false;
		    break;
    }
}

void SDLApplication::Init() {
    active = true;
	lastUpdate = SDL_GetTicks();
	nextUpdate = lastUpdate;
}

int SDLApplication::Quit() {
    SDL_Quit();
    return 0;
}

void SDLApplication::RegisterWindow(SDLWindow* window) {
    #ifdef IPHONE
    SDL_iPhoneSetAnimationCallback (window->sdlWindow, 1, UpdateFrame, NULL);
    #endif
}

void SDLApplication::SetFrameRate(double frameRate) {
    framePeriod = 1000.0 / frameRate;
}

SDLApplication::~SDLApplication() {
}
