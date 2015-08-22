#include "MainGame.h"

#include <Mirage/Mirage.h>

#include <SDL/SDL.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include <Mirage/vertex.h>
#include <Mirage/Timing.h>
#include <Mirage/errors.h>
#include <Mirage/ResourceManager.h>
#include <glm/gtx/rotate_vector.hpp>
#include <random>
#include <ctime>
#include "Zombie.h"
#include "Gun.h"
#include "Bullet.h"
MainGame::MainGame() : _screenWidth(800), _screenHeight(600), _gameState(GAME_STATE::PLAY), _fps(0), _player(nullptr), _numHumansKilled(0), _numZombiesKilled(0)
{
   
}

MainGame::~MainGame() 
{
   for(int i = 0; i < _levels.size(); i++)
   {
	   delete _levels[i];
   }

}

void MainGame::run() 
{
    initSystems();
	initLevel();

	Mirage::Music music = m_audioEngine.loadMusic("Sound/music.mp3");
    music.play(-1);

	gameLoop();
}

void MainGame::initSystems() 
{
    Mirage::init();
	// Initialize sound, must happen after Mirage::init
    m_audioEngine.init();
	_window.create("Zombie Apocalypse", _screenWidth, _screenHeight, 0);
	_window.setBackgroundColor(0.6f, 0.6f, 1.0f,1.0f);

	//initialize shaders
	initShaders();

	//initialize spritebatch
	_agentSpriteBatch.init();
	_hudSpriteBatch.init();

	//initialize sprite font
	_spriteFont = new Mirage::SpriteFont("Fonts/chintzy.ttf", 64);
	_camera.init(_screenWidth, _screenHeight);

	 // Initialize particles
    m_bloodParticleBatch = new Mirage::ParticleBatch2D;
    m_bloodParticleBatch->init(1000, 0.05f, 
								Mirage::ResourceManager::getTexture("Textures/particle.png"),
								//lamdas declaration
							[](Mirage::Particle2D& particle, float deltaTime) {
								particle.m_position += particle.m_velocity * deltaTime;
								particle.m_color.a = (GLubyte)(particle.m_life * 255.0f);
							}
							);

    m_particleEngine.addParticleBatch(m_bloodParticleBatch);

	
}
void MainGame::initLevel()
{
	//level 1
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;
	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPosition(),&_inputManager,&_camera, &_bullets);
	_humans.push_back(_player);
	
	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_real_distribution<float> randX(2, _levels[_currentLevel]->getWidth() - 2);
	std::uniform_real_distribution<float> randY(2, _levels[_currentLevel]->getHeight() - 2);


	
	//add all the random humans
	for(int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++)
	{
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		//glm::vec2 pos(10, 15);
		_humans.back()->init(HUMAN_SPEED, pos);
	}
	//add the zombies
	const std::vector<glm::ivec2>& zombiePosition = _levels[_currentLevel]->getZombieStartingPosition();
	
	for(int i = 0; i < zombiePosition.size(); i++)
	{
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePosition[i]);
	}
	//setup the player guns
	_player->addGun(new Gun("Magnum", 10, 1, 0.0f, 30, 20.0f, m_audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	_player->addGun(new Gun("ShotGun", 30, 12, 5.0f, 4, 20.0f, m_audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	_player->addGun(new Gun("MachineGun",2, 1, 0.0f, 20, 20.0f, m_audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
}

void MainGame::initShaders() 
{
    // Compile our color shader
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::gameLoop() 
{
	
   const float DESIRED_FLOAT = 60.0f;
   const int MAX_PHYSICS_STEPS = 6;
   Mirage::FpsLimiter fpsLimiter;

   fpsLimiter.setMaxFPS(60.0f);
   //zoomout by 3x
   const float CAMERA_SCALE = 1.0f/ 3.0f;
   _camera.setScale(CAMERA_SCALE);

   const float MILLISECONDS_PER_SECOND = 1000.0f;
   const float DESIRED_FRAME_TIME = MILLISECONDS_PER_SECOND / DESIRED_FLOAT;
   const float MAX_DELTA_TIME = 1;
   float previousTicks = SDL_GetTicks();

   //Main game
   while(_gameState == GAME_STATE::PLAY)
   {
	   fpsLimiter.begin();

	   float newTicks = SDL_GetTicks();
	   float frameTime = SDL_GetTicks() - previousTicks;
	   previousTicks = newTicks;
	   float totalDeltaTime = frameTime / DESIRED_FRAME_TIME;
	   checkVictory();
	   _inputManager.update();

	   processInput();
	   int i = 0;
	   while(totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
	   {
		   float deltaTime = std::min( totalDeltaTime, MAX_DELTA_TIME);
		   updateAgents(deltaTime);
		   updateBullets(deltaTime);
		   m_particleEngine.update(deltaTime);
		   totalDeltaTime -= deltaTime;
		   i++;
	   }

	   
	   _camera.setPosition(_player->getPosition());
	   _camera.update();
	   drawGame();
	   _fps = fpsLimiter.end();

   }

}
void MainGame::updateAgents(float deltaTime)
{
	//update all humans
	for(int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),
							_humans,
							_zombies, deltaTime);
	}

	//update all zombies
	for(int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(),
							_humans,
							_zombies, deltaTime);
	}

	//update human collisions
	for( int i = 0; i < _zombies.size(); i++)
	{
		//collide with other zombies
		for(int j = i + 1; j < _zombies.size(); j++)
		{
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		//collide with humans
		for(int j = 1; j < _humans.size(); j++)
		{
			//turn human into zombie by creating a new zombie in the humans position
			if(_zombies[i]->collideWithAgent(_humans[j]))
			{
				//add new zombies
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());

				//delete human
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}

		}
		//collide with player
		if(_zombies[i]->collideWithAgent(_player))
		{
			//Mirage::fatalError("YOU LOSE");
			std::cout<<"\nYou DIED!!\n";
				SDL_Quit();
		}

	}

	//update human collisions
	for( int i = 0; i < _humans.size(); i++)
	{
		//collide with other humans
		for(int j = i + 1; j < _humans.size(); j++)
		{
			_humans[i]->collideWithAgent(_humans[j]);
		}

	}

}

void MainGame::updateBullets(float deltaTime)
{
	//update and collide with world
	for(int i = 0; i < _bullets.size(); )
	{
		//if update returns true the bullet collided with wall
		if(_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime))
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}else 
			i++;

	}

	bool wasBulletRemoved;

	//collide with humans and zombies
	for(int i = 0; i < _bullets.size(); i++)
	{
		wasBulletRemoved = false;
		//loop through zombies
		for(int j = 0; j < _zombies.size(); )
		{
			//check collision
			if(_bullets[i].collideWithAgent(_zombies[j]))
			{
				addBlood(_bullets[i].getPosition(), 5);
				//make damage to zombie
				if(_zombies[j]->applyDamage(_bullets[i].getDamage()))
				{
					//if zombie died destroy
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}else 
					j++;
				//destroy bullet
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				i--;						///< make sure we dont skip a bullet
				break;						///< ending the loop since the bullet is destroyed, and iteration through all zombies is't required
			}else {
				j++;
				
			}
		}
		//loop through humans
		if(wasBulletRemoved == false){
			for(int j = 1; j < _humans.size(); )
			{
				//check collision
				if(_bullets[i].collideWithAgent(_humans[j]))
				{
					addBlood(_bullets[i].getPosition(), 5);
					//make damage to human
					if(_humans[j]->applyDamage(_bullets[i].getDamage()))
					{
						//if human died destroy
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_numHumansKilled++;
					}else 
						j++;
					//destroy bullet
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					wasBulletRemoved = true;
					i--;						///< make sure we dont skip a bullet
					break;						///< ending the loop since the bullet is destroyed, and iteration through all humans is't required
				}else {
					j++;
				
				}
			}
		}
	}
}

void MainGame::checkVictory()
{
	///we win if all zombies are dead
	if(_zombies.empty())
	{
		std::printf("You win!! ***\n You killed %d humans and %d zombies.\n There are %d/%d humans remaining",
					_numHumansKilled, _numZombiesKilled,_humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		Mirage::fatalError("");
	}

}

void MainGame::processInput() 
{
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                _gameState = GAME_STATE::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoordinates(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
                break;
        }
    }
}

void MainGame::drawGame() 
{
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();
    //drawing the game
	glActiveTexture(GL_TEXTURE0);						///< tells we want to use texture unit 0

	//making sure the shader uses texuture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
   
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();	///<grab the camera matrix
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//draw level
	_levels[_currentLevel]->draw();

	//begin drawing agents 
	_agentSpriteBatch.begin();

	//glm::vec2 agentPos;
	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);
	//draw humans
	for(int i = 0; i < _humans.size(); i++)
	{
		if(_camera.isObjectInView(_humans[i]->getPosition(), agentDims))
			_humans[i]->draw(_agentSpriteBatch);
	}

	//draw zombies
	for(int i = 0; i < _zombies.size(); i++)
	{
		if(_camera.isObjectInView(_humans[i]->getPosition(), agentDims))
			_zombies[i]->draw(_agentSpriteBatch);
	}
	//draw the bullets
	for(int i = 0; i < _bullets.size(); i++)
	{
		if(_camera.isObjectInView(_humans[i]->getPosition(), agentDims))
			_bullets[i].draw(_agentSpriteBatch);
	}
	_agentSpriteBatch.end();
	
	_agentSpriteBatch.renderBatch();

	// Render the particles
    m_particleEngine.draw(&_agentSpriteBatch);
	//draw hud
	//drawHud();
	_textureProgram.unUse();
    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}

void MainGame::drawHud()
{
	_hudSpriteBatch.begin();

	char buffer[256];
	
	sprintf_s(buffer, "Number of Humans %d", _humans.size());
	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(300, 300), 
						glm::vec2(4.0),  0.0f, Mirage::Color(255, 255, 255, 255));

	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}
void MainGame::addBlood(const glm::vec2& position, int numParticles) {

    static std::mt19937 randEngine(time(nullptr));
    static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

    glm::vec2 vel(2.0f, 0.0f);
    Mirage::Color col(255, 0, 0, 255);

    for (int i = 0; i < numParticles; i++) {
        m_bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine)), col, 20.0f);
    }
}