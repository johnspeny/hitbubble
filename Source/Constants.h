#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <string_view>
#include <string>

namespace category_bits
{
	// 1 2 4 8 16 32 64 128 256 // powers of 2
	inline constexpr short kAll = 1;
	inline constexpr short kBall = 2;
	inline constexpr short kWall = 4;
	inline constexpr short kWallLeft = 8;
	inline constexpr short kWallRight = 16;

	// entities
	inline constexpr short kHead = 32;
	inline constexpr short kTorso = 64;
	inline constexpr short kHand = 128;
	inline constexpr short kCeiling = 256;
	inline constexpr short kHair = 512;
	inline constexpr short kChain = 1024;

	inline constexpr short kBoard = 2048;
	inline constexpr short kItem = 4096;
	inline constexpr short kWallBushLeft = 8192;
	inline constexpr short kWallBushRight = 16384;
	inline constexpr short k3 = 32768;
	inline constexpr short k4 = 65536;
}

namespace sound_audio {
	// Sound Effects
	inline constexpr std::string_view btn_click = "audio/soundeffects/SmallFile3.mp3";
	inline constexpr std::string_view coin_collect = "audio/soundeffects/coinCollect.mp3";
	inline constexpr std::string_view head_ball = "audio/soundeffects/headBall.mp3";
	inline constexpr std::string_view applause = "audio/soundeffects/applause.mp3";
	inline constexpr std::string_view sad_crowd = "audio/soundeffects/sadcrowd.mp3";
	inline constexpr std::string_view thud = "audio/soundeffects/thud.mp3";
	inline constexpr std::string_view screams = "audio/soundeffects/screams.mp3";

	// Background Music
	inline constexpr std::string_view menu_bgm = "audio/background/background.mp3";
	//inline constexpr std::string_view game_bgm = "audio/background/background-music-aac.mp3";
	inline constexpr std::string_view game_bgm = "audio/background/bensound-cutesttoys.mp3";
}

namespace fonts {
	// fonts
	inline constexpr std::string_view baloobhai_ttf = "fonts/BalooBhai-Regular.ttf";
	inline constexpr std::string_view eng_cin_fnt = "fonts/enligsh-chinese.fnt";
	inline constexpr std::string_view hugmat_fnt = "fonts/hugmat.fnt";
	inline constexpr std::string_view supplycenter_fnt = "fonts/SupplyCenter.fnt";
}

namespace art
{
	// logo
	inline constexpr std::string_view logo = "logo/copy_holeygons.png";
	// spreadsheet collection
	inline constexpr std::string_view spritesheet = "gameassets/versiononespritesheet.plist";
	inline constexpr std::string_view twospritesheet = "gameassets/versiontwospritesheet.plist";
	// scene bg image
	inline constexpr std::string_view backgroundImage = "bg.png";
	inline const std::string backProggressImage = "bgPB.png";
	inline const std::string foreProggressImage = "forePB.png";
}

namespace game_data
{
	// seasons and levels
	inline constexpr std::string_view season_level_filepath = "data/season_level.json";
	// shot items
	inline constexpr std::string_view shop_items_filepath = "data/shop_item.json";
	// versions
	inline const std::string version_filepath = "version.txt";
	inline const std::string versionlevel_filepath = "versionLevel.txt";
}

namespace physics
{
	inline const float kGravity = -9.8f;
	inline constexpr int kVelocityIterations = 8;
	inline constexpr int kPositionIterations = 3;

	// player
	inline constexpr float kPlayerVel = 5.0f;

	// ball
	inline constexpr float kBallRadius = 20.0f;
}

namespace graphical_layers
{
	inline constexpr int kBackground = 1;
	inline constexpr int kPlayer = 2;
	inline constexpr int kBall = 3;
	inline constexpr int kRightBox = 4;
	inline constexpr int kLeftBush = 4;
	inline constexpr int kMeteor = 6;
	inline constexpr int kGround = 7;
}

#endif // !__CONSTANTS_H__