#pragma once

#include "../Practicas/ForceGenerators/ExplosionForceGenerator.h"
#include "../checkMemLeaks.h"

class RigidExplosionForceGenerator : public ExplosionForceGenerator {

public:
	RigidExplosionForceGenerator(float k, float r, double t, Vector3 origin, double maxLifetime = -1) : ExplosionForceGenerator(k, r, t, origin, maxLifetime) {
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

				((DRigidBody*)p)->addForce(f);
			}

		}
	};

	inline void setOrigin(Vector3 o) { origin_ = o; }
};