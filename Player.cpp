#include "Player.h"
#include <SDL/SDL.h>
#include "Gun.h"
Player::Player(void):_currentGunIndex(-1)
{
}


Player::~Player(void)
{
}
void Player::init(float speed, glm::vec2 position, Mirage::InputManager* inputManager, Mirage::Camera2D* camera, std::vector<Bullet>* bullets)
{
	_speed = speed;
	_position = position;
	_inputManager = inputManager;
	_bullets = bullets;
	_camera = camera;
	_health = 150.0f;
	_color.setColor(0, 0, 185, 255); 
}

void Player::addGun(Gun* gun)
{
	//add gun to inventory
	_guns.push_back(gun);
	//if no gun equiped, equip gun
	if(_currentGunIndex == -1)
	{
		_currentGunIndex = 0;
	}

}


void Player::update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies)
{
	if(_inputManager->isKeyPressed(SDLK_w))
	{
		_position.y += _speed;
	}
	else if(_inputManager->isKeyPressed(SDLK_s))
	{
		_position.y -= _speed;
	}

	if(_inputManager->isKeyPressed(SDLK_a))
	{
		_position.x -= _speed;
	}
	else if(_inputManager->isKeyPressed(SDLK_d))
	{
		_position.x += _speed;
	}

	if(_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0)
	{
		_currentGunIndex = 0;
	}else if(_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 0)
	{
		_currentGunIndex = 1;
	}else if(_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 0)
	{
		_currentGunIndex = 2;
	}

	if(_currentGunIndex != -1)
	{
		glm::vec2 mouseCoords = _inputManager->getMouseCoordinates();

		mouseCoords = _camera->convertScreenToWorld(mouseCoords);
		glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

		_guns[_currentGunIndex]->update(_inputManager->isKeyPressed(SDL_BUTTON_LEFT),
										centerPosition,
										direction,
										*_bullets);
	}


	collideWithLevel(levelData);
}
