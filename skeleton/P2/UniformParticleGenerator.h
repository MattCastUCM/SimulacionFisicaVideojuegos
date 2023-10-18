#pragma once

#include "ParticleGenerator.h"
class UniformParticleGenerator : public ParticleGenerator {
protected:
	//Vector3 velWidth_, posWidth_;
	std::uniform_real_distribution<double> distrUnif_{ 0, 1 };


public:
	UniformParticleGenerator(const std::string& name, double min, double max) : ParticleGenerator(name) { 
		
		distrUnif_.param(std::uniform_real_distribution<double>::param_type(min, max));
	};

	std::list<Particle*> generateParticles() override {
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			
			auto velMagn = p->getInitVel().magnitude();
			auto velX = distrUnif_(mt_);
			auto velZ = distrUnif_(mt_);
			auto vel = p->getInitVel();
			vel.x = velX * velMagn;
			vel.z = velZ * velMagn;
			p->setVel({ vel.x, vel.y, vel.z });


			generated.push_back(p);
		}

		return generated;
	};

	
};