#pragma once

#include "../Generators/GaussianParticleGenerator.h"

class CircleGenerator : public GaussianParticleGenerator {
public:
	// Media y desviación estándar
	CircleGenerator(double genTime, double mean, double dev, bool autoInactive) : GaussianParticleGenerator(genTime, mean, dev, autoInactive) { };

	inline std::list<Particle*> generateParticles() override {
		std::list<Particle*> generated;
		Particle* p;
		float rnd = normDistr_(mt_);
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			setVelocities(p, i, rnd);

			generated.push_back(p);
		}
		return generated;
	};


	inline void setVelocities(Particle* p, int i, float rnd) {
		p->setInitPos(origin_);
		p->setPos(origin_);

		auto velMagn = p->getInitVel().magnitude();
		float velX = p->getInitVel().x,
			  velY = p->getInitVel().y,
			  velZ = p->getInitVel().z;
		
		float a = (360 / generateN_) * i;
		velX = physx::PxCos(rads(a)) * velMagn + velMagn * rnd;
		velZ = physx::PxSin(rads(a)) * velMagn + velMagn * rnd;
		velY = 2 * velMagn + velMagn * rnd;

		p->setVel({ velX, velY, velZ });
	}



};