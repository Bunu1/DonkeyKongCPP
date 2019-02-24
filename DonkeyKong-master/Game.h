#pragma once
#include "Entity.h"
#define ECHELLE_COUNT 4
#define BLOCK_COUNT_X 8
#define BLOCK_COUNT_Y 5
#define BLOCK_SPACE 110.f
#define FLAME_ENEMY_COUNT 5


class Game
{
public:
	Game();
	~Game() { };
	void run();

public:
	int levelMatrix[40][40];

private:
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();
	void updateStatistics(sf::Time elapsedTime);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	bool groundIsUnder();
	std::shared_ptr<Entity> initialiseEntityClass(std::string);
	EntityType initialiseEntityType(std::string);
	void drawSprite();
	void setLayout();
	float getCountablePos(sf::Sprite, std::string);

private:
	static const float		PlayerSpeed;
	static const float		EnemySpeed;
	static const float		FallSpeed;
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	sf::Texture	mTexture;
	sf::Sprite	mPlayer;
	sf::Font	mFont;
	sf::Text	mStatisticsText;
	sf::Time	mStatisticsUpdateTime;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;

	std::vector<std::string> drawables = { "Coin", "Donkey", "Flame_Enemy", "Ground", "Ladder", "Mario" };
	std::vector<sf::Texture> textures;
	std::vector<sf::Sprite> sprites;
	std::vector<sf::Vector2u> sprites_sizes;

	sf::Texture	_TextureEchelle;
	sf::Sprite	_Echelle[ECHELLE_COUNT];
	sf::Texture	_TextureBlock;
	sf::Sprite	_Block[BLOCK_COUNT_X][BLOCK_COUNT_Y];
	sf::Texture	_TextureWeapon;
	sf::Sprite	_Weapon;
	sf::Vector2u _sizeMario;
};

