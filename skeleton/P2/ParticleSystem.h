#pragma once

#include <list>
#include "../P1/Particle.h"
#include "ParticleGenerator.h"

class ParticleSystem {
protected:
	const double PART_TIME_ = 1.0f;
	double time;
	std::list<Particle*> particles_;
	std::list<ParticleGenerator*> generators_;

	Vector3 gravity_;
	void refresh();

public:
	// Se usa -10.0f como gravedad por defecto
	ParticleSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f });
	~ParticleSystem();
	
	virtual void update(double t);

	//ParticleGenerator* getParticleGenerator(const std::string& name);


};

