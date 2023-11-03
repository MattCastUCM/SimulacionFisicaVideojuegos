#pragma once

#include "Particle.h"
#include <string>

class ForceGenerator {
protected:
	std::string name_;
	double t_ = 0.0; // If starting negative --> eternal
	double duration_ = -1e10;


public:
	ForceGenerator() { } 
	virtual ~ForceGenerator() { }
	
	inline bool updateTime(double t) {
		t_ += t;
		return t_ < duration_ || duration_ < 0.0; // Devuelve true si ya era cero o si es positivo
	}
	virtual void update(Particle* p, double t) { };
};