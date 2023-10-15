#pragma once

#include "ParticleGenerator.h"

class UniformParticleGenerator : ParticleGenerator {
private:
	Vector3 velWidth_, posWidth_;

public:
	UniformParticleGenerator(const std::string& name) : ParticleGenerator(name) { };

	std::list<Particle*> generateParticles() override {
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			generated.push_back(p);
		}

		return generated;
	};

	
};