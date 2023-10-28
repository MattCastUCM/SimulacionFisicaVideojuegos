#pragma once

#include "../Generators/GaussianParticleGenerator.h"

class CircleGenerator : public GaussianParticleGenerator {
public:
	// Media y desviación estándar
	CircleGenerator(double genTime, double mean, double dev, int offset, bool autoInactive) 
		: GaussianParticleGenerator(genTime, mean, dev, offset, autoInactive) { };

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

		auto velMagn = vel_.magnitude();
		
		auto vel = vel_;
		float a = (360 / generateN_) * i;
		vel.x = physx::PxCos(rads(a)) * velMagn + velMagn * rnd;
		vel.y *= velMagn * abs(rnd) * 5;
		vel.z = physx::PxSin(rads(a)) * velMagn + velMagn * rnd;
		p->setVel(vel);
	}



};