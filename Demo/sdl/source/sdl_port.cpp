#include "sdl_port.h"

#include <stdio.h>
#include <algorithm>

#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include "tb_msg.h"
#include "tb_system.h"

SDL_TimerID gTimer = -1;
static Uint32 sdl_timer_callback(Uint32 interval, void *param)
{
	if (gTimer != -1) {
		SDL_RemoveTimer(gTimer);
		gTimer = -1;
	}

	double next_fire_time = tb::TBMessageHandler::GetNextMessageFireTime();
	double now = tb::TBSystem::GetTimeMS();
	if (now < next_fire_time)
	{
		// We timed out *before* we were supposed to (the OS is not playing nice).
		// Calling ProcessMessages now won't achieve a thing so force a reschedule
		// of the platform timer again with the same time.
		ReschedulePlatformTimer(next_fire_time, true);
		return 0;
	}

	tb::TBMessageHandler::ProcessMessages();

	// If we still have things to do (because we didn't process all messages,
	// or because there are new messages), we need to rescedule, so call RescheduleTimer.
	tb::TBSystem::RescheduleTimer(tb::TBMessageHandler::GetNextMessageFireTime());
	return 0;
}

void ReschedulePlatformTimer(double fire_time, bool force)
{
	static double set_fire_time = -1;
	if (fire_time == TB_NOT_SOON)
	{
		set_fire_time = -1;
		if (gTimer != -1) {
			SDL_RemoveTimer(gTimer);
			gTimer = -1;
		}
	}
	else if (fire_time != set_fire_time || force || fire_time == 0)
	{
		set_fire_time = fire_time;
		double delay = fire_time - tb::TBSystem::GetTimeMS();
		unsigned int idelay = (unsigned int)std::max(delay, 0.0);
		gTimer = SDL_AddTimer(idelay, sdl_timer_callback, nullptr);
	}
}

void tb::TBSystem::RescheduleTimer(double fire_time)
{
	ReschedulePlatformTimer(fire_time, false);
}