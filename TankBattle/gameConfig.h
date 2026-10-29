#ifndef __GAME_CONFIG_H

#define __GAME_CONFIG_H

#define PLAYERS_NUM 2

class GameConfig
{
public :

	enum class eKeys { U , UR  , R , DR  , D  , DL  , L  , UL ,S ,W, ESC = 27 , Z = 'Z', z = 'z', M = 'M', m = 'm', INVALID = -1 };

	static constexpr int GAME_WIDTH = 80;
	static constexpr int GAME_HEIGHT = 25;

	static constexpr int MIN_X = 1;

	static constexpr int MIN_Y=1;
};

#endif
