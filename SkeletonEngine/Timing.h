#pragma once

namespace SkeletonEngine
{
	class FpsLimiter
	{
	public:
		FpsLimiter();
		void init(float max_fps);
		void setMaxFPS(float max_fps);

		void begin();
		float end();
	private:
		float max_fps_;
		unsigned int start_ticks_;
		float fps_;
		float frame_time_;

		void calculateFPS();
	};
}
