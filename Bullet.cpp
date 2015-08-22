#include "Bullet.h"
#include <Mirage/ResourceManager.h>
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"
Bullet::Bullet(glm::vec2 position, glm::vec2 direction,float damage, float speed):
	_position(position),
	_direction(direction),
	_damage(damage),
	_speed(speed)
{
}


Bullet::~Bullet(void)
{
}
bool Bullet::update(const std::vector<std::string>& levelData, float deltTime)
{
	_position += _direction * _speed * deltTime;
	return collideWithWorld(levelData);
}

bool Bullet::collideWithAgent(Agent* agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;
	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB= agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;

	if(collisionDepth > 0){

		return true;
	}
	return false;
}
bool Bullet::collideWithWorld(const std::vector<std::string>& levelData)
{
	glm::ivec2 gripPosition(floor(_position.x/ (float)TILE_WIDTH), floor(_position.y/ (float)TILE_WIDTH));
	//return if outside the world
	if( gripPosition.x < 0 || 
		gripPosition.x >= levelData[0].size() ||
		gripPosition.y < 0 || 
		gripPosition.y >= levelData[0].size())
		{
			return true;
		}

	return (levelData[gripPosition.y][gripPosition.x] != '.');
	
}


void Bullet::draw(Mirage::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect(	_position.x + BULLET_RADIUS,
						_position.y + BULLET_RADIUS,
						BULLET_RADIUS * 2,
						BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Mirage::Color color;
	color.setColor(255, 255, 255, 255);
	spriteBatch.draw(destRect, uvRect, Mirage::ResourceManager::getTexture("Textures/circle.PNG").id, 0, color);
}
