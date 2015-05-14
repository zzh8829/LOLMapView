#ifndef Z_TIMER_H_
#define Z_TIMER_H_

#include "Core.h"

#include "SDL2/SDL.h"

class Timer
{
public:
	Timer();

	static uint32_t GetTimeInMilliseconds()
	{
		return SDL_GetTicks();
	}
	static float GetTimeInSeconds()
	{
		return SDL_GetTicks()/1000.0f;
	}
};

#endif