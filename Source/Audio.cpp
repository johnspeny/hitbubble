#include "Audio.h"
#include "audio/AudioEngine.h"

USING_NS_AX;

namespace
{
	AUDIO_ID bgmAudioID{};
}

Audio& Audio::getInstance()
{
	static Audio instance;
	return instance;
}

void Audio::playSfx(const std::string& fileName)
{
	AudioEngine::play2d(fileName, false);
}

void Audio::playBgm(const std::string& fileName)
{
	if (bgmAudioID)
	{
		resumeBgm(bgmAudioID);
		//AudioEngine::resume(bgmAudioID);
		//stopBgm();
		//bgmAudioID = {};
	}
	else
	{
		bgmAudioID = AudioEngine::play2d(fileName, true);
	}
}

void Audio::playBgm(const std::string& fileName, float vol)
{
	if (bgmAudioID)
	{
		resumeBgm(bgmAudioID);
		//AudioEngine::resume(bgmAudioID);
		//stopBgm();
		//bgmAudioID = {};
	}
	else
	{
		bgmAudioID = AudioEngine::play2d(fileName, true, vol);
	}
}

void Audio::stopBgm()
{
	AudioEngine::stop(bgmAudioID);
	bgmAudioID = {};
}

void Audio::resumeBgm(int id)
{
	AudioEngine::resume(id);
}