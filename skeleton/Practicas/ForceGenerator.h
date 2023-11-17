#pragma once

#include "Particle.h"
#include <string>

class ForceGenerator {
protected:
	std::string name_;
	double timeConstant_ = 0.0; // If starting negative --> eternal
	double duration_ = -1e10;


public:
	ForceGenerator(std::string name = "") : name_(name) { }
	virtual ~ForceGenerator() { }
	
	inline bool updateTime(double t) {
		timeConstant_ += t;
		return timeConstant_ < duration_ || duration_ < 0.0; // Devuelve true si ya era cero o si es positivo
	}
	virtual void update(Particle* p, double t) { };

	inline void setName(std::string name) { name_ = name; }
	inline std::string getTame() { return name_; }
};