#include "Human.h"
#include <ctime>
#include <random>
#include <Mirage/ResourceManager.h>
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
	_color.setColor(255, 255, 255, 255);
	_speed = speed;
	_position = position;

	//get random direction
	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));

	//making sure direction is't zero
	if(m_direction.length() == 0)
	{
		m_direction = glm::vec2(1.0f, 0.0f);
	}
	m_direction = glm::normalize(m_direction);
	m_textureID =  Mirage::ResourceManager::getTexture("Textures/human.png").id;
}


void Human::update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies,
						float deltaTime)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);
	_position += m_direction * _speed * deltaTime;

	//randomly change direction per 500 frames
	if(_frames == 500){
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
		_frames = 0;
	}else 
		_frames++;

	if(collideWithLevel(levelData))
	{
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
	}

}

