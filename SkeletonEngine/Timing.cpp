#include "Timing.h"

#include <SDL.h>

namespace SkeletonEngine
{
	FpsLimiter::FpsLimiter()
	{
	}

	void FpsLimiter::init(float max_fps)
	{
		setMaxFPS(max_fps);
	}

	void FpsLimiter::setMaxFPS(float max_fps)
	{
		max_fps_ = max_fps;
	}
	
	void FpsLimiter::begin()
	{
		start_ticks_ = SDL_GetTicks();
	}

	float FpsLimiter::end()
	{
		calculateFPS();

		// limit fps to max fps
		float frame_ticks = SDL_GetTicks() - start_ticks_;
		if (1000.0f / max_fps_ > frame_ticks)
		{
			SDL_Delay(1000.0f / max_fps_ - frame_ticks);
		}

		return fps_;
	}

	void FpsLimiter::calculateFPS()
	{
		static const int NUM_SAMPLES = 10;
		static float frame_times[NUM_SAMPLES];
		static int curr_frame = 0;

		static float prev_ticks = SDL_GetTicks();
		float curr_ticks;
		curr_ticks = SDL_GetTicks();

		frame_time_ = curr_ticks - prev_ticks;
		frame_times[curr_frame % NUM_SAMPLES] = frame_time_;

		prev_ticks = curr_ticks;

		curr_frame++;
		int count;
		if (curr_frame < NUM_SAMPLES)
		{
			count = curr_frame;
		}
		else
		{
			count = NUM_SAMPLES;
		}

		float frame_time_avg = 0;
		for (size_t i = 0; i < count; i++)
		{
			frame_time_avg += frame_times[i];
		}
		frame_time_avg /= static_cast<float> (count);

		if (frame_time_avg > 0)
		{
			fps_ = 1000.0f / frame_time_avg;
		}
		else
		{
			fps_ = 60.0f;
		}
	}

}
