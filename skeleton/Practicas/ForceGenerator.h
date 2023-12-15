#pragma once

#include "Particle.h"
#include <string>

class ForceGenerator {
protected:
	std::string name_;
	double lifeTime_ = 0.0; // If starting negative --> eternal
	double maxLifeTime_ = -1e10;


public:
	ForceGenerator(std::string name = "", double maxLifeTime = -1) : name_(name), lifeTime_(0.0), maxLifeTime_(maxLifeTime) { }

	virtual ~ForceGenerator() { }
	
	inline void updateTime(double t) { lifeTime_ += t; }
	inline double getLifeTime() { return lifeTime_; }
	inline bool isActive() { 
		return lifeTime_ < maxLifeTime_ || maxLifeTime_ < 0; }
	inline void resetTime() { lifeTime_ = 0; }


	virtual void update(Particle* p, double t) { };

	inline void setName(std::string name) { name_ = name; }
	inline std::string getName() { return name_; }

	inline virtual void keyPress(unsigned char key) { }
};