#pragma once

#include <Mirage/Window.h>
#include <Mirage/GLSLProgram.h>
#include <Mirage/Camera2D.h>
#include <Mirage/InputManager.h>
#include <Mirage/SpriteBatch.h>

#include "Level.h"
#include <vector>
#include "Human.h"
#include "Player.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.5f;
const float PLAYER_SPEED = 8.0f;
enum class GAME_STATE
{
	PLAY,
	EXIT
};

class Zombie;
class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
	 /// Member Variables
    Mirage::Window _window; ///< The game window
    
    Mirage::GLSLProgram _textureProgram; ///< The shader program

    Mirage::InputManager _inputManager; ///< Handles input

    Mirage::Camera2D _camera; ///< Main Camera
	
	Mirage::SpriteBatch _agentSpriteBatch;		///< draws all agents

	std::vector<Level*> _levels;		///< vector of all levels in the game
	
	Player* _player;

	std::vector<Human*> _humans;		///< vector of humans

	std::vector<Zombie*> _zombies;		///< vector of _zombies

	std::vector<Bullet> _bullets;
	
	int _screenWidth, _screenHeight;
	
	GAME_STATE _gameState;

	float _fps;

	int _numHumansKilled; ///< humans killed by player

	int _numZombiesKilled; ///< zombies killed by player

	int _currentLevel;

private:
    /// Initializes the core systems
    void initSystems();

	/// Initializes level and sets up everything
	void initLevel();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program  
    void gameLoop();

	///updates all agents
	void updateAgents();
	
	///update bullets
	void updateBullets();

	///checks victory condition
	void checkVictory();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

   
};

