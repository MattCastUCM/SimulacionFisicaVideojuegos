#pragma once

#include "ParticleGenerator.h"
class GaussianParticleGenerator : public ParticleGenerator {
protected:
	//Vector3 velWidth_, posWidth_;
	std::normal_distribution<double> distrNorm_{ 0, 1 };


public:
	GaussianParticleGenerator(const std::string& name, double media, double desv) : ParticleGenerator(name) {
		distrNorm_.param(std::normal_distribution<double>::param_type(media, desv));
	};

	std::list<Particle*> generateParticles() override {
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();

			auto velMagn = p->getInitVel().magnitude();
			auto velX = distrNorm_(mt_);
			auto velZ = distrNorm_(mt_);
			auto vel = p->getInitVel();
			vel.x = velX * velMagn;
			vel.z = velZ * velMagn;
			p->setVel({ vel.x, vel.y, vel.z });


			generated.push_back(p);
		}

		return generated;
	};


};