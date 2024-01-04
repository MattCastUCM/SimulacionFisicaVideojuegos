#pragma once

#include "../ForceGenerator.h"
#include <cmath>
#include "../../checkMemLeaks.h"

class ExplosionForceGenerator : public ForceGenerator {
protected:
	const float SOUNDVEL_ = 342.2f;
	float k_, explR_, timeConstant_;
	Vector3 origin_;

public:
	ExplosionForceGenerator(float k, float r, double t, Vector3 origin, double maxLifetime = -1) : ForceGenerator("", maxLifetime), k_(k), explR_(r), origin_(origin) {
		timeConstant_ = t;
	};

	virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			auto pos = p->getPos();
			auto dist = pos - origin_;
			auto rad = dist.magnitude();
			explR_ = SOUNDVEL_ * lifeTime_;
			if ( rad < explR_ && rad != 0) {
				float intensity = k_ / (rad * rad);
				float eN = exp(-lifeTime_ / timeConstant_);	// exp(n) = e^n

				auto f = intensity * dist * eN;
				p->addForce(f);
			}

		}
	};
};