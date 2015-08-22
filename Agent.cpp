#include "Agent.h"
#include "Level.h"
#include <Mirage/ResourceManager.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
Agent::Agent(void) : m_direction(1.0f, 0.0f)
{
}


Agent::~Agent(void)
{
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData)
{
		std::vector<glm::vec2> collideTilePositions;

		//check the four corners
		//first corner
		checkTilePosition(levelData,
						  collideTilePositions,
						  _position.x,
						  _position.y);

		//second corner
		checkTilePosition(levelData,
						  collideTilePositions,
						  _position.x + AGENT_WIDTH,
						  _position.y);
		//third corner
		checkTilePosition(levelData,
						  collideTilePositions,
						  _position.x ,
						  _position.y + AGENT_WIDTH);
		//fourth corner
		checkTilePosition(levelData,
						  collideTilePositions,
						  _position.x + AGENT_WIDTH,
						  _position.y + AGENT_WIDTH);

		if(collideTilePositions.size() == 0)
		{
			return false;
		}

		//do the collision
		for(int i = 0; i < collideTilePositions.size(); i++)
		{
			collideWithTile(collideTilePositions[i]);
		}
		return true;
}

bool Agent::collideWithAgent(Agent* agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS * 2;
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB= agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;

	if(collisionDepth > 0){
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;

		_position += collisionDepthVec/2.0f;		//pushing both by half
		agent->_position -= collisionDepthVec/2.0f;

		return true;
	}
	return false;
		
}


void Agent::draw(Mirage::SpriteBatch& _spriteBatch)
{
	
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 desRect;
	desRect.x = _position.x;
	desRect.y = _position.y;
	desRect.z = AGENT_WIDTH;
	desRect.w = AGENT_WIDTH;

	_spriteBatch.draw(desRect, uvRect, m_textureID, 0.0f, _color, m_direction);
}

bool Agent::applyDamage(float damage)
{
	_health -= damage;
	if(_health <= 0)
	{
		return true;
	}
	return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData,
							  std::vector<glm::vec2>& collideTilePositions,
						      float x,
						      float y)
{
	//check the four courners
	glm::vec2 cornerPos = glm::vec2(
										floor(x / (float) TILE_WIDTH),
										floor(y / (float) TILE_WIDTH));	

	//return if outside the world
	if( cornerPos.x < 0 || 
		cornerPos.x >= levelData[0].size() ||
		cornerPos.y < 0 || 
		cornerPos.y >= levelData[0].size())
	{
		return;
	}

	if(levelData[cornerPos.y][cornerPos.x] != '.'){
		collideTilePositions.push_back(cornerPos * (float) TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}

}
//Access a line bounding box collision
void Agent::collideWithTile(glm::vec2 tilePosition)
{
	
	const float TILE_RADIUS = (float) TILE_WIDTH/2.0f;
	//minimum distance before collision occurs
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	
	//center position of agent
	glm::vec2 centrePlayerPos = _position + glm::vec2(AGENT_RADIUS);

	//vector from agent to tile
	glm::vec2 distVec = centrePlayerPos - tilePosition;
	
	//get the depth of collision
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	//if this is true we're colliding
	if(xDepth > 0 || yDepth > 0)
	{
		if(std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
		{
			if(distVec.x < 0)
				_position.x -= xDepth;
			else
				_position.x += xDepth;
		}else {
			if(distVec.y < 0)
				_position.y -= yDepth;
			else 
				_position.y += yDepth;

		}

	}

}

