#pragma once

enum RenderEventType {
		RENDER,
		RENDER_CONTEXT_LOST,
		RENDER_CONTEXT_RESTORED
};

struct RenderEvent {
	RenderEventType type;

	static void (*callback)(RenderEvent* event);
	static void Dispatch(RenderEvent* event);
    RenderEvent();
};
