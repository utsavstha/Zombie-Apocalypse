#pragma once

#include <glm/glm.hpp>
#include <Mirage/SpriteBatch.h>
#include <vector>
const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH/2.0f;
class Human;
class Zombie;

class Agent
{
public:
	Agent(void);
	virtual ~Agent(void);
	void draw(Mirage::SpriteBatch& _spriteBatch);
	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies) = 0;
	glm::vec2 getPosition() const {return _position; }
	bool collideWithLevel(const std::vector<std::string>& levelData);		//returns true if collided with level
	bool collideWithAgent(Agent* agent);
	//returns true if dead
	bool applyDamage(float damage);
	
protected:
	void checkTilePosition(const std::vector<std::string>& levelData,
						   std::vector<glm::vec2>& collideTilePositions,
						   float x,
						   float y);
	void collideWithTile(glm::vec2 tilePosition);
	glm::vec2 _position;
	float _speed;
	float _health;
	Mirage::Color _color;
};

