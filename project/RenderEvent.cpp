#include "RenderEvent.h"

void (*RenderEvent::callback)(RenderEvent* event) = 0;

RenderEvent::RenderEvent() {
    type = RENDER;
}

void RenderEvent::Dispatch(RenderEvent* event) {
    if(callback) {
		callback(event);
    }
}
