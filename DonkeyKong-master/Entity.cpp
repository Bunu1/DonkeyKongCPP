#include "pch.h"
#include "Entity.h"


Entity::Entity(sf::Sprite _m_sprite, sf::Vector2u _m_size, sf::Vector2f _m_position, int _speed, int _max_speed, int _quickening)
{
	m_sprite = _m_sprite;
	m_size = _m_size;
	m_position = _m_position;
	setSpeed(_speed);
	setMax_Speed(_max_speed);
	setQuickening(_quickening);
}

int Entity::getSpeed() 
{
	return speed;
}
int Entity::getMax_Speed()
{
	return max_speed;
}
int Entity::getQuickening()
{
	return quickening;
}
bool Entity::getM_Enabled()
{
	return m_enabled;
}
void Entity::setSpeed(int _speed)
{
	speed = _speed;
}
void Entity::setMax_Speed(int _max_speed)
{
	max_speed = _max_speed;
}
void Entity::setQuickening(int _quickening)
{
	quickening = _quickening;
}
void Entity::setM_Enabled(bool _m_enabled)
{
	m_enabled = _m_enabled;
}