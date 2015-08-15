#pragma once
#include <glm/glm.hpp>
#include <Mirage/SpriteBatch.h>
#include <vector>
class Human;
class Zombie;
class Agent;
const int BULLET_RADIUS = 8;
class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction,float damage, float speed);
	~Bullet(void);
	//when update returns true delete bullet
	bool update(const std::vector<std::string>& levelData);
	void draw(Mirage::SpriteBatch& spriteBatch);
	bool collideWithAgent(Agent* agent);
	float getDamage() const { return _damage; }
private:
	bool collideWithWorld(const std::vector<std::string>& levelData);
	float _damage;
	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;
};

