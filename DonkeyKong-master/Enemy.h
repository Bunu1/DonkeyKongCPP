#pragma once
#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
	bool getM_bLeftToRight();
	int getM_times();
	void setM_bLeftToRight(bool);
	void setM_times(int);


public:
	bool m_bLeftToRight = true;
	int m_times = 0;
};

