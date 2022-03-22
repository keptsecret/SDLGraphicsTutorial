#pragma once

#include <map>
#include <string>
#include <SDL/SDL_mixer.h>

namespace SkeletonEngine {

	class SoundEffect
	{
	public:
		friend class AudioEngine;		// only AudioEngine can modify private variables

		void play(int loops = 0);		// -1 to loop forever, or play loops+1 times

	private:
		Mix_Chunk* chunk_ = nullptr;
	};

	class Music
	{
	public:
		friend class AudioEngine;

		void play(int loops = -1);		// -1 to loop forever, or play loops times

		// these affect the current playing track, not a specific Music
		static void pause();
		static void stop();
		static void resume();

	private:
		Mix_Music* music_ = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& file_path);
		Music loadMusic(const std::string& file_path);

	private:
		bool is_initialized_ = false;
		std::map<std::string, Mix_Chunk*> effect_map_;
		std::map<std::string, Mix_Music*> music_map_;
	};

}
