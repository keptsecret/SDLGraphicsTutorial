#include "AudioEngine.h"

#include "SkellyErrors.h"

namespace SkeletonEngine
{
	void SoundEffect::play(int loops)
	{
		if (Mix_PlayChannel(-1, chunk_, loops) == -1)
		{
			// we might be out of channels, force onto channel 0
			if (Mix_PlayChannel(0, chunk_, loops) == -1)
			{
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loops)
	{
		Mix_PlayMusic(music_, loops);		
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine()
	{
	}

	AudioEngine::~AudioEngine()
	{
		destroy();
	}

	void AudioEngine::init()
	{
		if (is_initialized_)
		{
			fatalError("Tried to initialize AudioEngine twice");
		}

		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
		{
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
		}

		is_initialized_ = true;
	}

	void AudioEngine::destroy()
	{
		if (is_initialized_)
		{
			is_initialized_ = false;

			for (auto& it : effect_map_)
			{
				Mix_FreeChunk(it.second);
			}

			for (auto& it : music_map_)
			{
				Mix_FreeMusic(it.second);
			}

			effect_map_.clear();
			music_map_.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const std::string& file_path)
	{
		// find audio in cache
		auto it = effect_map_.find(file_path);

		SoundEffect effect;

		if (it == effect_map_.end())
		{
			// failed to load
			Mix_Chunk* chunk = Mix_LoadWAV(file_path.c_str());
			if (chunk == nullptr) {
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}

			effect.chunk_ = chunk;
			effect_map_[file_path] = chunk;
		}
		else
		{
			// already cached
			effect.chunk_ = it->second;
		}

		return effect;
	}

	Music AudioEngine::loadMusic(const std::string& file_path)
	{
		// find audio in cache
		auto it = music_map_.find(file_path);

		Music music;

		if (it == music_map_.end())
		{
			// failed to load
			Mix_Music* chunk = Mix_LoadMUS(file_path.c_str());
			if (chunk == nullptr) {
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}

			music.music_ = chunk;
			music_map_[file_path] = chunk;
		}
		else
		{
			// already cached
			music.music_ = it->second;
		}

		return music;
	}
}
