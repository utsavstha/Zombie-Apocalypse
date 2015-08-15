#pragma once
#include <string>
#include "Bullet.h"
#include <glm/glm.hpp>
#include <vector>
class Gun
{
public:
	 Gun(std::string name, int fireRate, int _bulletsPerShot, float spread, float bulletDamage, float bulletSpeed);
	~Gun(void);

	void update(bool isMouseDown,const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);
private:
	std::string _name;
	int _fireRate;			///< fire rate in terms of frames
	int	_bulletsPerShot;	///< number of bullets fired at a time
	float _spread;			///< accuracy
	float _bulletSpeed;		///< bullets velocity
	float _bulletDamage;	
	int _frameCounter;

private:
	void shoot(const glm::vec2& direction,const glm::vec2& position, std::vector<Bullet>& bullets);
};

