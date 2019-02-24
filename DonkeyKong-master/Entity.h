#pragma once

enum EntityType
{
	Player,
	Environment,
	Enemy,
	Coin,
	Donkey,
	Flame_Enemy,
	Ground,
	Ladder,
	Mario
};


class Entity
{
public:
	Entity() { };
	~Entity() { };
	Entity(sf::Sprite, sf::Vector2u, sf::Vector2f, int, int, int);
	int getSpeed();
	int getMax_Speed();
	int getQuickening();
	bool getM_Enabled();
	void setSpeed(int);
	void setMax_Speed(int);
	void setQuickening(int);
	void setM_Enabled(bool);


public:
	sf::Sprite m_sprite;
	sf::Vector2u m_size;
	sf::Vector2f m_position;
	int speed;
	int max_speed;
	int quickening;
	EntityType m_type;
	bool m_enabled = true;

	// Enemy only
	bool m_bLeftToRight = true;
	int m_times = 0;
};
