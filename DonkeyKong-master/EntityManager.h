#pragma once
#include "Entity.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

public:
	static std::vector<std::shared_ptr<Entity>> m_Entities;
	static std::vector<std::shared_ptr<Entity>> GetEntitiesGroup(std::string);
};

