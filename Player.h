#pragma once
#include "Human.h"
#include "Bullet.h"
#include <Mirage/InputManager.h>
#include <Mirage/Camera2D.h>
class Gun;
class Player : public Human
{
public:
	Player(void);
	~Player(void);

	void init(float speed, glm::vec2 position, Mirage::InputManager* _inputManager, Mirage::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData,
				std::vector<Human*>& humans,
				std::vector<Zombie*>& zombies,
				float deltaTime) override;///< move player based on keyboard input
private:
	Mirage::InputManager* _inputManager;
	std::vector<Gun*> _guns;
	int _currentGunIndex;
	Mirage::Camera2D* _camera;
	std::vector<Bullet>* _bullets;
};

