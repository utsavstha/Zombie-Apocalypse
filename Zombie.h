#pragma once
#include "Agent.h"


class Zombie : public Agent
{
public:
	Zombie(void);
	~Zombie(void);

	void init(float speed, glm::vec2 position);

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies,
						float deltaTime)override;
private:
	Human* getNearestHuman(std::vector<Human*>& humans);
};

