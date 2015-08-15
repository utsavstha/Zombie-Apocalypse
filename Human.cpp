#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
Human::Human(void) : _frames(0)
{
}


Human::~Human(void)
{
}

void Human::init(float speed, glm::vec2 position)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	_health = 20.0f;
	_color.setColor(127, 60, 200, 255);
	_speed = speed;
	_position = position;

	//get random direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));

	//making sure direction is't zero
	if(_direction.length() == 0)
	{
		_direction = glm::vec2(1.0f, 0.0f);
	}
	_direction = glm::normalize(_direction);
}


void Human::update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);
	_position += _direction * _speed;

	//randomly change direction per 500 frames
	if(_frames == 500){
		_direction = glm::rotate(_direction, randRotate(randomEngine));
		_frames = 0;
	}else 
		_frames++;

	if(collideWithLevel(levelData))
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}

}

