#pragma once

#include <list>
#include "../P1/Particle.h"

class ParticleSystem {
protected:
	std::list<Particle*> particles_;
	//std::list<ParticleGenerator*> generators_;

	Vector3 gravity_;
	void refresh();

public:
	ParticleSystem(const Vector3& g = { 0.0f, -9.8f, 0.0f });
	~ParticleSystem();
	
	virtual void update(double t);

	//ParticleGenerator* getParticleGenerator(const std::string& name);


};

