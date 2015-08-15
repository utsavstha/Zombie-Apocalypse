#include "Level.h"
#include <Mirage/errors.h>
#include <fstream>
#include <iostream>
#include <Mirage/ResourceManager.h>
Level::Level(const std::string& fileName)
{
	
	std::ifstream file;
	file.open(fileName);

	//error checking
	if(file.fail())
	{
		Mirage::fatalError("failed to open" + fileName);
	}
	std::string tmp;
	file >> tmp >> _numOfHumans;		//adding first string in tmp

	std::getline(file, tmp);			///< Throw away the first line

	//read level data
	while(std::getline(file,tmp))
		_levelData.push_back(tmp);

	_spriteBatch.init();
	_spriteBatch.begin();
	
	glm::vec4 uvRect (0.0f, 0.0f ,1.0f, 1.0f); 
	Mirage::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;
	//render all the tiles
	for(int y = 0; y < _levelData.size(); y++)
	{
		for(int x = 0; x < _levelData[y].size(); x++)
		{
			//grab the tile
			char tile = _levelData[y][x];

			//get desitination rectangle
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			//process the tile
			switch (tile)
			{
			case 'B':
			case 'R':
				_spriteBatch.draw(destRect, 
								  uvRect, 
								  Mirage::ResourceManager::getTexture("Textures/red_bricks.png").id,
								  0.0f,
								  whiteColor);

				break;
			
			case 'G':
				_spriteBatch.draw(destRect, 
								uvRect, 
								Mirage::ResourceManager::getTexture("Textures/glass.png").id,
								0.0f,
								whiteColor);

				break;

			case 'L':
				_spriteBatch.draw(destRect, 
								uvRect, 
								Mirage::ResourceManager::getTexture("Textures/light_bricks.png").id,
								0.0f,
								whiteColor);

				break;

			case '.':
				_spriteBatch.draw(destRect, 
								uvRect, 
								Mirage::ResourceManager::getTexture("Textures/ground.png").id,
								0.0f,
								whiteColor);

				break;

			case '@':
				_levelData[y][x] = '.';
				_spriteBatch.draw(destRect, 
								uvRect, 
								Mirage::ResourceManager::getTexture("Textures/ground.png").id,
								0.0f,
								whiteColor);
				_startingPosition.x = x * TILE_WIDTH;
				_startingPosition.y = y * TILE_WIDTH;
				break;

			case 'Z':
				_levelData[y][x] = '.';
				_spriteBatch.draw(destRect, 
								uvRect, 
								Mirage::ResourceManager::getTexture("Textures/ground.png").id,
								0.0f,
								whiteColor);
				_zombieStartingPosition.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;

			default:
				std::printf("unexped symbol %c at (%d, %d)", tile, x, y);
				break;
			}
		}

	}

	_spriteBatch.end();

}


Level::~Level(void)
{
}

void Level::draw()
{
	_spriteBatch.renderBatch();
}

