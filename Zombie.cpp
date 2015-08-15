#include "Zombie.h"
#include <glm/glm.hpp>
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
	_health = 150.0f;
	_position = position;
	_color.setColor(80, 160, 140, 255);
}

void Zombie::update(const std::vector<std::string>& levelData,
					std::vector<Human*>& humans,
					std::vector<Zombie*>& zombies)
{
	Human* closestHuman = getNearestHuman(humans);
	if(closestHuman != nullptr)
	{
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction * _speed;
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
