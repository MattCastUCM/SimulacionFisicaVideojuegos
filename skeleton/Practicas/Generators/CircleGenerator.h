#pragma once

#include "GaussianParticleGenerator.h"

class CircleGenerator : public GaussianParticleGenerator {
public:
	// Media y desviación estándar
	CircleGenerator(double genTime, double mean, double dev) : GaussianParticleGenerator(genTime, mean, dev) { };

	inline std::list<Particle*> generateParticles() override {
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			setVelocities(p);

			generated.push_back(p);
		}
		active_ = false;
		return generated;
	};


	inline void setVelocities(Particle* p) {
		p->setInitPos(origin_);
		p->setPos(origin_);

		auto velMagn = p->getInitVel().magnitude();
		float velX = p->getInitVel().x,
			 velZ = p->getInitVel().z;
		if (changeX_) velX = normDistr_(mt_) * velMagn;
		if (changeZ_) velZ = normDistr_(mt_) * velMagn;
		p->setVel({ velX, 0.0f, velZ });
	}
};