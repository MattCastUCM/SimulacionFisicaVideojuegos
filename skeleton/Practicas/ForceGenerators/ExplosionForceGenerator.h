#pragma once

#include "../ForceGenerator.h"
#include <cmath>

class ExplosionForceGenerator : public ForceGenerator {
protected:
	const float SOUNDVEL_ = 342.2f;
	float k_, explR_, timeConstant_, elapsedTime_;
	Vector3 origin_;

public:
	ExplosionForceGenerator(float k, float r, float t, Vector3 origin) : ForceGenerator(), k_(k), explR_(r), timeConstant_(t), origin_(origin), elapsedTime_(0) { };
	
	inline void setK(const float& k) { k_ = k; }
	inline float getK() const { return k_; }
	inline void resetTime() { elapsedTime_ = 0; }

	virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			elapsedTime_ += t;
			auto pos = p->getPos();
			auto dist = pos - origin_;
			auto rad = dist.magnitude();
			explR_ = SOUNDVEL_ * elapsedTime_;
			if ( rad < explR_) {
				float intensity = k_ / (rad * rad);
				float eN = exp(-elapsedTime_ / timeConstant_);	// exp(n) = e^n

				auto f = intensity * dist * eN;
				p->addForce(f);
			}

		}
	};
};