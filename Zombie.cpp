#include "Zombie.h"
#include <glm/glm.hpp>
#include <Mirage/ResourceManager.h>
#include "Human.h"
Zombie::Zombie(void)
{
}


Zombie::~Zombie(void)
{
}
void Zombie::init(float speed, glm::vec2 position)
{
	_speed = speed;
	_health = 80.0f;
	_position = position;
	_color.setColor(255, 255, 255, 255);
	m_textureID =  Mirage::ResourceManager::getTexture("Textures/zombie.png").id;
}

void Zombie::update(const std::vector<std::string>& levelData,
					std::vector<Human*>& humans,
					std::vector<Zombie*>& zombies,
					float deltaTime)
{
	Human* closestHuman = getNearestHuman(humans);
	if(closestHuman != nullptr)
	{
		m_direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += m_direction * _speed * deltaTime;
	}

	collideWithLevel(levelData);

}
Human* Zombie::getNearestHuman(std::vector<Human*>& humans)
{
	Human* closestHuman = nullptr;		///< if returning nullptr there are no humans
	float smallestDistance = 99999;

	for(int i = 0; i < humans.size(); i++)		///< loop through all the humans
	{
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);

		if(distance < smallestDistance)
		{
			smallestDistance = distance;
			closestHuman = humans[i];
		}


	}
	return closestHuman;
}
