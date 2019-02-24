#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "Coin.h"
#include "Donkey.h"
#include "Flame_Enemy.h"
#include "Ground.h"
#include "Ladder.h"
#include "Mario.h"

const float Game::PlayerSpeed = 100.f;
const float Game::EnemySpeed = 50.f;
const float Game::FallSpeed = 75.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(840, 600), "Donkey Kong 1981", sf::Style::Close)
	, mTexture()
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
{
	mWindow.setFramerateLimit(160);
	
	//Load level from configuration file
	setLayout();

	// Load Sprites, Texures and Sizes
	for (int i = 0; i < std::size(drawables); i++)
	{
		std::stringstream ss;
		sf::Texture texture;
		texture.loadFromFile("Media/Textures/SQ" + drawables[i] + ".png");

		sf::Vector2u size;
		size = texture.getSize();

		sf::Sprite sprite;
		sprite.setTexture(texture);

		textures.push_back(texture);
		sprites_sizes.push_back(size);
		sprites.push_back(sprite);
	}
	drawSprite();
	// Draw Statistic Font 

	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setString("Welcome to Donkey Kong 1981");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	sf::Vector2f movement(0.f, 0.f);
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

bool Game::groundIsUnder() {
	sf::FloatRect ground;
	for (std::shared_ptr<Entity> mario : EntityManager::GetEntitiesGroup("Player"))
	{
		sf::FloatRect posMario = mario->m_sprite.getGlobalBounds();
		//posMario.y = mario->m_sprite.getPosition().y;
		for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Environment"))
		{
			if (entity->m_type == EntityType::Ground || entity->m_type == EntityType::Ladder)
			{
				//ground.x = entity->m_sprite.getPosition().x;
				//ground.y = entity->m_sprite.getPosition().y;
				//std::cout << (int)(ground.y - posMario.y) % 15 << "\n";

				ground = entity->m_sprite.getGlobalBounds();

				/*if (abs((int)(ground.y - posMario.y) % 15) == 2)
					if(abs((int)(ground.x - posMario.x)) < 42)
						return true;*/
				if(ground.intersects(posMario))return true;
			}
		}
	}
	return false;
}


float Game::getCountablePos(sf::Sprite sp, std::string s) {
	sf::Vector2f position = sp.getPosition();
	if (s == "x")
	{
		return position.x;
	}
	else if (s == "y")
	{
		return position.y;
	}
	return 0.f;
}

void Game::update(sf::Time elapsedTime)
{
	sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if (mIsMovingRight)
		movement.x += PlayerSpeed;

	//gravity
	if(!groundIsUnder())
		movement.y += FallSpeed;
	if (groundIsUnder())
		std::cout << "SAAS" << "\n";

	for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Player"))
	{
		if (entity->m_enabled)
		{
			entity->m_sprite.move(movement * elapsedTime.asSeconds());
			entity->m_sprite.setPosition(abs((int)entity->m_sprite.getPosition().x % 840), abs((int)entity->m_sprite.getPosition().y % 600));
		}
		
	}
	for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Environment"))
	{
		//if(entity->m_type == EntityType::Donkey)
		//std::cout << entity->m_type << "\n";
	}
	for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Enemy"))
	{
		if (entity->m_type == EntityType::Donkey)
		{

		}
		else {

		}
		//if(entity->m_type == EntityType::Donkey)
		//std::cout << entity->m_type << "\n";
	}
}

void Game::render()
{
	mWindow.clear();

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		mWindow.draw(entity->m_sprite);
	}

	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}

	//
	// Handle collision
	//

	if (mStatisticsUpdateTime >= sf::seconds(0.050f))
	{
		// Handle collision weapon enemies
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::Up)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::Down)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Left)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::Right)
		mIsMovingRight = isPressed;

	if (key == sf::Keyboard::Space)
	{
	}
}

void Game::setLayout()
{
	std::string STRING;
	std::ifstream infile("level.ini");
	int line = 0;
	while (getline(infile, STRING))
	{
		int col = 0;
		for (int i = 0; i < STRING.length(); i=i+2) {
			levelMatrix[line][col] = STRING[i] - '0';

			col++;
		}
		line++;
	}
	infile.close();
} 

std::shared_ptr<Entity> Game::initialiseEntityClass(std::string s) {
	if (s == "Coin")
	{
		return std::make_shared<class Coin>();
	}
	else if (s == "Donkey")
	{
		return std::make_shared<class Donkey>();
	}
	else if (s == "Flame_Enemy")
	{
		return std::make_shared<class Flame_Enemy>();
	}
	else if (s == "Ground")
	{
		return std::make_shared<class Ground>();
	}
	else if (s == "Ladder")
	{
		return std::make_shared<class Ladder>();
	}
	else if (s == "Mario")
	{
		return std::make_shared<class Mario>();
	}
	return std::make_shared<class Entity>();
}

EntityType Game::initialiseEntityType(std::string s)
{
	if (s == "Coin")
	{
		return EntityType::Coin;
	}
	else if (s == "Donkey")
	{
		return EntityType::Donkey;
	}
	else if (s == "Flame_Enemy")
	{
		return EntityType::Flame_Enemy;
	}
	else if (s == "Ground")
	{
		return EntityType::Ground;
	}
	else if (s == "Ladder")
	{
		return EntityType::Ladder;
	}
	else if (s == "Mario")
	{
		return EntityType::Mario;
	}
	return EntityType::Ground;
}

void Game::drawSprite()
{
	//std::cout << sizeof(levelMatrix) / sizeof(levelMatrix[0]) << " ";
	for (int i = 0; i < (sizeof(levelMatrix) / sizeof(levelMatrix[0])); i++)
	{
		for (int j = 0; j < (sizeof(levelMatrix) / sizeof(levelMatrix[0])); j++)
		{
			if (levelMatrix[i][j] > 0) 
			{
				if (drawables[levelMatrix[i][j] - 1] == "Coin")
				{
					std::shared_ptr<class Coin> se = std::make_shared<class Coin>();
					sprites[levelMatrix[i][j] - 1].setTexture(textures[levelMatrix[i][j] - 1]);
					sprites[levelMatrix[i][j] - 1].setPosition(21.f * j, 15.f * i);
					se->m_sprite = sprites[levelMatrix[i][j] - 1];
					se->m_type = initialiseEntityType(drawables[levelMatrix[i][j] - 1]);
					se->m_size = sprites_sizes[levelMatrix[i][j] - 1];
					se->m_position = sprites[levelMatrix[i][j] - 1].getPosition();
					EntityManager::m_Entities.push_back(se);
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Donkey")
				{
					std::shared_ptr<class Donkey> se = std::make_shared<class Donkey>();
					sprites[levelMatrix[i][j] - 1].setTexture(textures[levelMatrix[i][j] - 1]);
					sprites[levelMatrix[i][j] - 1].setPosition(21.f * j, 15.f * i);
					se->m_sprite = sprites[levelMatrix[i][j] - 1];
					se->m_type = initialiseEntityType(drawables[levelMatrix[i][j] - 1]);
					se->m_size = sprites_sizes[levelMatrix[i][j] - 1];
					se->m_position = sprites[levelMatrix[i][j] - 1].getPosition();
					EntityManager::m_Entities.push_back(se);
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Flame_Enemy")
				{
					std::shared_ptr<class Flame_Enemy> se = std::make_shared<class Flame_Enemy>();
					sprites[levelMatrix[i][j] - 1].setTexture(textures[levelMatrix[i][j] - 1]);
					sprites[levelMatrix[i][j] - 1].setPosition(21.f * j, 15.f * i);
					se->m_sprite = sprites[levelMatrix[i][j] - 1];
					se->m_type = initialiseEntityType(drawables[levelMatrix[i][j] - 1]);
					se->m_size = sprites_sizes[levelMatrix[i][j] - 1];
					se->m_position = sprites[levelMatrix[i][j] - 1].getPosition();
					EntityManager::m_Entities.push_back(se);
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Ground")
				{
					std::shared_ptr<class Ground> se = std::make_shared<class Ground>();
					sprites[levelMatrix[i][j] - 1].setTexture(textures[levelMatrix[i][j] - 1]);
					sprites[levelMatrix[i][j] - 1].setPosition(21.f * j, 15.f * i);
					se->m_sprite = sprites[levelMatrix[i][j] - 1];
					se->m_type = initialiseEntityType(drawables[levelMatrix[i][j] - 1]);
					se->m_size = sprites_sizes[levelMatrix[i][j] - 1];
					se->m_position = sprites[levelMatrix[i][j] - 1].getPosition();
					EntityManager::m_Entities.push_back(se);
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Ladder")
				{
					std::shared_ptr<class Ladder> se = std::make_shared<class Ladder>();
					sprites[levelMatrix[i][j] - 1].setTexture(textures[levelMatrix[i][j] - 1]);
					sprites[levelMatrix[i][j] - 1].setPosition(21.f * j, 15.f * i);
					se->m_sprite = sprites[levelMatrix[i][j] - 1];
					se->m_type = initialiseEntityType(drawables[levelMatrix[i][j] - 1]);
					se->m_size = sprites_sizes[levelMatrix[i][j] - 1];
					se->m_position = sprites[levelMatrix[i][j] - 1].getPosition();
					EntityManager::m_Entities.push_back(se);
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Mario")
				{
					
					std::shared_ptr<class Ladder> se = std::make_shared<class Ladder>();
					sprites[levelMatrix[i][j] - 1].setTexture(textures[levelMatrix[i][j] - 1]);
					sprites[levelMatrix[i][j] - 1].setPosition(21.f * j, 15.f * i);
					se->m_sprite = sprites[levelMatrix[i][j] - 1];
					se->m_type = initialiseEntityType(drawables[levelMatrix[i][j] - 1]);
					se->m_size = sprites_sizes[levelMatrix[i][j] - 1];
					se->m_position = sprites[levelMatrix[i][j] - 1].getPosition();
					EntityManager::m_Entities.push_back(se);
				}
			}
		}
	}
}