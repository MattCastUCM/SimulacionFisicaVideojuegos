#pragma once

#include "../ParticleGenerator.h"

class CircleGenerator : public ParticleGenerator {
private:
	float rads(float a) { return 0.017453292 * a; }
	double rads(double a) { return 0.017453292 * a; }


public:
	// Media y desviación estándar
	CircleGenerator(double genTime, double mean, double dev) : ParticleGenerator(genTime) { };

	inline std::list<Particle*> generateParticles() override {
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			setVelocities(p, i);

			generated.push_back(p);
		}
		active_ = false;
		return generated;
	};


	inline void setVelocities(Particle* p, int i) {
		p->setInitPos(origin_);
		p->setPos(origin_);

		auto velMagn = p->getInitVel().magnitude();
		float velX = p->getInitVel().x,
			 velZ = p->getInitVel().z;
		
		float a = 360 / generateN_;
		velX = physx::PxCos(rads(a * i)) * velMagn;
		velZ = physx::PxSin(rads(a * i)) * velMagn;
		p->setVel({ velX, 0.0f, velZ });
	}



};