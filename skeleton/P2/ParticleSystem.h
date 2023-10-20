#pragma once

#include "../P1/Particle.h"
#include "ParticleGenerator.h"

#include <unordered_map>

class ParticleSystem {
protected:
	Vector3 gravity_;

	std::vector<Particle*> particles_;
	std::unordered_map<std::string, ParticleGenerator*> generators_;

	void refresh();

public:
	// Se usa -10.0f como gravedad por defecto
	ParticleSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f });
	~ParticleSystem();
	
	virtual void update(double t);

	ParticleGenerator* getParticleGenerator(const std::string& name);


};

