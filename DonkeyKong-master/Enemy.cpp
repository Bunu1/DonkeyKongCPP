#include "pch.h"
#include "Enemy.h"


Enemy::Enemy()
{
}

bool Enemy::getM_bLeftToRight()
{
	return m_bLeftToRight;
}
int Enemy::getM_times()
{
	return m_times;
}
void Enemy::setM_bLeftToRight(bool _m_bLeftToRight)
{
	m_bLeftToRight = _m_bLeftToRight;
}
void Enemy::setM_times(int _m_times)
{
	m_times = _m_times;
}


Enemy::~Enemy()
{
}
