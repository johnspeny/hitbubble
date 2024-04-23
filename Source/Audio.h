#pragma once

#include <string>

class Audio
{
public:
	static Audio& getInstance();

	void playSfx(const std::string& fileName);
	void playBgm(const std::string& fileName);
	void playBgm(const std::string& fileName, float vol);
	void stopBgm();
	void resumeBgm(int id);

private:
	Audio() = default;
};
