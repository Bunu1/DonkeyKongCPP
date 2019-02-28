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

const float Game::PlayerSpeed = 135.f;
const float Game::EnemySpeed = 50.f;
const float Game::FallSpeed = 200.f;
const float Game::JumpSpeed = 10.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(840, 600), "Donkey Kong 1981", sf::Style::Close)
	, mTexture()
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
{
	mWindow.setFramerateLimit(160);
	
	//Load level from configuration file
	setLayout();

	// Load Sprites, Texures and Sizes
	for (int i = 0; i < std::size(drawables); i++)
	{
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
	loadEndAnims();
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
		for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Environment"))
		{
			if (entity->m_type == EntityType::Ground)
			{
				ground = entity->m_sprite.getGlobalBounds();
				if(ground.intersects(posMario))return true;
			}
		}
	}
	return false;
}

bool Game::isNearLadder() {
	sf::FloatRect ground;
	for (std::shared_ptr<Entity> mario : EntityManager::GetEntitiesGroup("Player"))
	{
		sf::FloatRect posMario = mario->m_sprite.getGlobalBounds();
		for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Environment"))
		{
			if (entity->m_type == EntityType::Ladder)
			{
				ground = entity->m_sprite.getGlobalBounds();
				if (ground.intersects(posMario))return true;
			}
		}
	}
	return false;
}

bool Game::foeGravity() {
	sf::FloatRect ground;
	int i = 0;
	for (std::shared_ptr<Entity> mario : EntityManager::GetEntitiesGroup("Enemy"))
	{

		sf::FloatRect posMario = mario->m_sprite.getGlobalBounds();
		if (mario->m_type != EntityType::Donkey)
		{
			for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Environment"))
			{

				if (entity->m_type == EntityType::Ground)
				{
					ground = entity->m_sprite.getGlobalBounds();
					if (ground.intersects(posMario))
					{
						if (!i)i++;
						else
							return true;
					}
				}
			}
		}
		
	}
	return false;
}

bool Game::playerVsFoe() {
	sf::FloatRect ground;
	for (std::shared_ptr<Entity> mario : EntityManager::GetEntitiesGroup("Player"))
	{
		sf::FloatRect posMario = mario->m_sprite.getGlobalBounds();
		for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Enemy"))
		{
			ground = entity->m_sprite.getGlobalBounds();
			if (ground.intersects(posMario))return true;
		}
	}
	return false;
}

bool Game::playerVsGoal() {
	sf::FloatRect ground;
	for (std::shared_ptr<Entity> mario : EntityManager::GetEntitiesGroup("Player"))
	{
		sf::FloatRect posMario = mario->m_sprite.getGlobalBounds();
		for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Environment"))
		{
			if (entity->m_type == EntityType::Coin)
			{
				ground = entity->m_sprite.getGlobalBounds();
				if (ground.intersects(posMario))return true;
			}
		}
	}
	return false;
}

void Game::deathAnim(sf::Time elapsedTime) {
	sf::Vector2f movement(210.f, 0.f);
	for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Environment"))
	{
		if (entity->m_type == EntityType::Ends)
		{
			
			if (entity->m_sprite.getPosition().x > 840)
			{
				movement.x -= 84000.f;
				entity->m_sprite.move(movement * elapsedTime.asSeconds());
				sf::Vector2f movementMario(0.f, 0.f);
				for (std::shared_ptr<Entity> mario : EntityManager::GetEntitiesGroup("Player"))
				{
					mario->m_sprite.setPosition(30.f, 520.f);
				}
				gameEnded = false;

			}
			else {
				entity->m_sprite.move(movement * elapsedTime.asSeconds());
				gameEnded = true;
			}
			break;
		}
	}
}

void Game::winAnim(sf::Time elapsedTime) {
	sf::Vector2f movement(210.f, 0.f);
	int i = 0;
	for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Environment"))
	{
		if (entity->m_type == EntityType::Ends)
		{
			if (!i)
			{
				i++;
				continue;
			}

			if (entity->m_sprite.getPosition().x > 840)
			{
				movement.x -= 84000.f;
				entity->m_sprite.move(movement * elapsedTime.asSeconds());
				sf::Vector2f movementMario(0.f, 0.f);
				for (std::shared_ptr<Entity> mario : EntityManager::GetEntitiesGroup("Player"))
				{
					mario->m_sprite.setPosition(30.f, 520.f);
				}
				gameEnded = false;

			}
			else {
				entity->m_sprite.move(movement * elapsedTime.asSeconds());
				gameEnded = true;
			}
			break;
		}
	}
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
	sf::Vector2f movementFoe(0.f, 0.f);
	sf::Vector2f movementDonkey(0.f, 0.f);

	if (playerVsFoe())deathAnim(elapsedTime);
	if (playerVsGoal())winAnim(elapsedTime);

	if (gameEnded)return;

	//gravity Mario
	if (!groundIsUnder())
	{
		if (mIsMovingUp)
			movement.y -= PlayerSpeed;
		movement.y += FallSpeed;
	}

	//Mario movement
	if (mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if (mIsMovingRight)
		movement.x += PlayerSpeed;

	//Ladder movements Mario
	if (isNearLadder())
	{
		if (mIsMovingUp)
			movement.y -= PlayerSpeed;
		if (mIsMovingDown && !groundIsUnder())
			movement.y += PlayerSpeed;
	}

	//Jumping Mario
	if (mIsJumping > 0)
	{
		movement.y -= JumpSpeed * mIsJumping;
		mIsJumping--;
	}

	//Donkey Jumping
	if (dJumping > 0)
	{
		if (dJumping > 20) 
		{
			movementDonkey.y -= JumpSpeed * (dJumping-20);
		}
		else {
			movementDonkey.y += JumpSpeed * (dJumping);
		}

		dJumping--;
	}
	else {
		dJumping = 40;
	}

	//foeMovement
	if (foeLoop > 0)
	{
		if (foeLoop > 80)
		{
			if (foeLoop > 120)
			{
				movementFoe.y += JumpSpeed / 4 * (foeLoop - 120);
			}
			else
			{
				movementFoe.y -= JumpSpeed / 4 * (foeLoop - 80);
			}
			movementFoe.x -= JumpSpeed/2 * (foeLoop - 80);
		}
		else {
			if (foeLoop > 40)
			{
				movementFoe.y -= JumpSpeed / 4 * (foeLoop - 40);
			}
			else
			{
				movementFoe.y += JumpSpeed / 4 * foeLoop ;
			}
			movementFoe.x += JumpSpeed/2 * (foeLoop);
		}

		foeLoop--;
	}
	else {
		foeLoop = 160;
	}

	for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Player"))
	{
		if (entity->m_enabled)
		{

			entity->m_sprite.setOrigin({ entity->m_sprite.getLocalBounds().width / 2, 0 });
			if (mIsMovingLeft) {
				entity->m_sprite.setScale({ -1, 1 });
			}
			if (mIsMovingRight) {
				entity->m_sprite.setScale({ 1, 1 });
			}

			entity->m_sprite.move(movement * elapsedTime.asSeconds());
			if(entity->m_sprite.getPosition().x > 840 || entity->m_sprite.getPosition().x < 0)
				entity->m_sprite.setPosition(abs((int)entity->m_sprite.getPosition().x % 840), entity->m_sprite.getPosition().y);
			if(entity->m_sprite.getPosition().y > 600 || entity->m_sprite.getPosition().y < 0)
				entity->m_sprite.setPosition(entity->m_sprite.getPosition().x, abs((int)entity->m_sprite.getPosition().y % 600));
		}
		
	}
	for (std::shared_ptr<Entity> entity : EntityManager::GetEntitiesGroup("Enemy"))
	{
		if (entity->m_type == EntityType::Donkey)
		{
			entity->m_sprite.move(movementDonkey * elapsedTime.asSeconds());
		}
		else {
			entity->m_sprite.move(movementFoe * elapsedTime.asSeconds());
		}
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

	if (key == sf::Keyboard::Space && groundIsUnder())
	{
		mIsJumping = 40;
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
	for (int i = 0; i < (sizeof(levelMatrix) / sizeof(levelMatrix[0])); i++)
	{
		for (int j = 0; j < (sizeof(levelMatrix) / sizeof(levelMatrix[0])); j++)
		{
			if (levelMatrix[i][j] > 0) 
			{
				std::shared_ptr<class Entity> se;
				if (drawables[levelMatrix[i][j] - 1] == "Coin")
				{
					se = std::make_shared<class Coin>();
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Donkey")
				{
					se = std::make_shared<class Donkey>();
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Flame_Enemy")
				{
					se = std::make_shared<class Flame_Enemy>();
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Ground")
				{
					se = std::make_shared<class Ground>();
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Ladder")
				{
					se = std::make_shared<class Ladder>();
				}
				else if (drawables[levelMatrix[i][j] - 1] == "Mario")
				{	
					se = std::make_shared<class Ladder>();
				}

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

	//end game sprites
	sprites[6].setTexture(textures[6]);
	sprites[6].setPosition(-420.f,150.f);
	std::shared_ptr<class Entity> se = std::make_shared<class Environment>();
	se->m_sprite = sprites[6];
	se->m_type = EntityType::Ends;
	se->m_size = sprites_sizes[6];
	se->m_position = sprites[6].getPosition();
	//se->m_enabled = false;
	EntityManager::m_Entities.push_back(se);

	sprites[7].setTexture(textures[7]);
	sprites[7].setPosition(-420.f, 150.f);
	std::shared_ptr<class Entity> se2 = std::make_shared<class Environment>();
	se2->m_sprite = sprites[7];
	se2->m_type = EntityType::Ends;
	se2->m_size = sprites_sizes[7];
	se2->m_position = sprites[7].getPosition();
	//se2->m_enabled = false;
	EntityManager::m_Entities.push_back(se2);
}

void Game::loadEndAnims()
{
	sf::Texture texture;
	texture.loadFromFile("Media/Textures/SQend.jpg");
	sf::Vector2u size = texture.getSize();
	sf::Sprite sprite;
	sprite.setTexture(texture);
	textures.push_back(texture);
	sprites_sizes.push_back(size);
	sprites.push_back(sprite);

	sf::Texture texture2;
	texture2.loadFromFile("Media/Textures/SQwin.jpg");
	sf::Vector2u size2 = texture2.getSize();
	sf::Sprite sprite2;
	sprite2.setTexture(texture2);
	textures.push_back(texture2);
	sprites_sizes.push_back(size2);
	sprites.push_back(sprite2);
}