#include "Gun.h"
#include <ctime>
#include <random>
#include <Mirage/AudioEngine.h>
#include <glm/gtx/rotate_vector.hpp>
Gun::Gun(std::string name, int fireRate, int _bulletsPerShot, float spread, float bulletDamage, float bulletSpeed, Mirage::SoundEffect fireEffect):
	_name(name),
	_fireRate(fireRate),
	_bulletsPerShot(_bulletsPerShot),
	_spread(spread),
	_bulletDamage(bulletDamage),
	_bulletSpeed(bulletSpeed),
	_frameCounter(0),
	m_fireEffect(fireEffect)
{
}


Gun::~Gun(void)
{
}

void Gun::update(bool isMouseDown,const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime)
{
	_frameCounter += 1.0f * deltaTime;
	if(_frameCounter >= _fireRate && isMouseDown)
	{
		shoot(direction ,position, bullets);
		_frameCounter = 0;
	}

}


void Gun::shoot(const glm::vec2& direction, const glm::vec2& position,std::vector<Bullet>& bullets)
{
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRot(-_spread, _spread);

	m_fireEffect.play();
	for(int i = 0; i < _bulletsPerShot; i++)
	{
		bullets.emplace_back(position, 
							glm::rotate(direction,randRot(randomEngine)), 
							_bulletDamage,
							_bulletSpeed);
	}

}
