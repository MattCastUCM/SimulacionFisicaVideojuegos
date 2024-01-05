#pragma once

#include "Particle.h"
#include <string>
#include "../checkMemLeaks.h"

class ForceGenerator {
protected:
	std::string name_;
	double lifeTime_ = 0.0; // If starting negative --> eternal
	double maxLifeTime_ = -1e10;
	bool active_ = true, autoInactive_ = false;

public:
	ForceGenerator(std::string name = "", double maxLifeTime = -1, bool autoInactive = false) 
		: name_(name), lifeTime_(0.0), maxLifeTime_(maxLifeTime), autoInactive_(autoInactive), active_(false) { }

	virtual ~ForceGenerator() { }
	
	inline void updateTime(double t) { if (active_) lifeTime_ += t; }
	inline void updateAlive() {
		if ((lifeTime_ < maxLifeTime_ || maxLifeTime_ < 0) && autoInactive_)
			active_ = false;
	}
	inline double getLifeTime() { return lifeTime_; }
	inline void resetTime() { lifeTime_ = 0; }
	
	inline void setActive(bool active = false) { active_ = active; }
	inline virtual bool isActive() { return active_; }


	virtual void update(Particle* p, double t) { };

	inline void setName(std::string name) { name_ = name; }
	inline std::string getName() { return name_; }

	inline virtual void keyPress(unsigned char key) { }
};