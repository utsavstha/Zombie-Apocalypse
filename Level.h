#pragma once
#include <string>
#include <vector>
#include <Mirage/SpriteBatch.h>

const int TILE_WIDTH = 64;
class Level
{
public:
	//build and load the level
	Level(const std::string& fileName);
	~Level(void);
	//getters
	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }
	int getNumHumans() const { return _numOfHumans; }
	glm::vec2 getStartPlayerPosition() const { return _startingPosition; }
	const std::vector<glm::ivec2>& getZombieStartingPosition() const { return _zombieStartingPosition; }
	void draw();

	//getters
	const std::vector<std::string>& getLevelData() const { return _levelData; }
private:
	std::vector<std::string> _levelData;
	int _numOfHumans;
	Mirage::SpriteBatch _spriteBatch;
	glm::vec2 _startingPosition;					///< starting position for the player read from level data
	std::vector<glm::ivec2> _zombieStartingPosition;		///< starting position for the zombies read from level data
};

