#pragma once

#include "../ParticleGenerator.h"

class HeartGenerator : public ParticleGenerator {
public:
	// Media y desviación estándar
	HeartGenerator(double genTime, double mean, double dev, bool autoInactive) : ParticleGenerator(genTime, autoInactive) { };

	inline std::list<Particle*> generateParticles() override {
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			setVelocities(p, i);

			generated.push_back(p);
		}
		return generated;
	};


	inline void setVelocities(Particle* p, int i) {
		p->setInitPos(origin_);
		p->setPos(origin_);

		auto velMagn = vel_.magnitude();
		auto vel = vel_;
		
		float a = 2 * physx::PxPi * i / generateN_;
		float sin = physx::PxSin(a);
		vel.x = 16 *  sin * sin * sin;;
		vel.y = 13 * physx::PxCos(a)
				- 5 * physx::PxCos(2 * a)
				- 2 * physx::PxCos(3 * a)
				- physx::PxCos(4 * a);
		vel.z = 0.0f;

		p->setVel(vel);
	}


	
};