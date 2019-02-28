#include "pch.h"
#include "EntityManager.h"

std::vector<std::shared_ptr<Entity>> EntityManager::m_Entities;

EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}

/*std::shared_ptr<Entity> EntityManager::GetPlayer()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type == EntityType::Mario)
		{
			return entity;
		}
	}

	return nullptr;
}*/

std::vector<std::shared_ptr<Entity>> EntityManager::GetEntitiesGroup(std::string s)
{
	std::vector<std::shared_ptr<Entity>> ents;
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}
		if (s == "Environment")
		{
			if (entity->m_type == EntityType::Ground || entity->m_type == EntityType::Ladder || entity->m_type == EntityType::Coin || entity->m_type == EntityType::Ends)
			{
				ents.push_back(entity);
			}
		}
		if (s == "Player")
		{
			if (entity->m_type == EntityType::Mario)
			{
				ents.push_back(entity);
			}
		}
		if (s == "Enemy")
		{
			if (entity->m_type == EntityType::Flame_Enemy || entity->m_type == EntityType::Donkey)
			{
				ents.push_back(entity);
			}
		}
		
	}

	return ents;
}
