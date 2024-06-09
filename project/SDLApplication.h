#pragma once

#include "app/Application.h"
#include "SDLWindow.h"
#include "RenderEvent.h"

#include <stdint.h>

class SDLApplication : public Application {
    public:
		SDLApplication ();
		~SDLApplication ();

		virtual int Exec ();
		virtual void Init ();
		virtual int Quit ();
		virtual void SetFrameRate (double frameRate);
		virtual bool Update ();

		void RegisterWindow (SDLWindow* window);

		static SDLApplication* currentApplication;

    private:
		bool active;
		uint32_t initFlags;
		uint32_t currentUpdate;
		uint32_t lastUpdate;
		uint32_t nextUpdate;
		double framePeriod;

		RenderEvent renderEvent;

		void HandleEvent (SDL_Event* event);

		static void UpdateFrame ();
		static void UpdateFrame (void*);
};
