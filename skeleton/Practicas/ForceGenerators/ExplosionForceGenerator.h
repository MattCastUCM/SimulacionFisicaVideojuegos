#pragma once

#include "../ForceGenerator.h"
#include <cmath>
#define _USE_MATH_DEFINES

class ExplosionForceGenerator : public ForceGenerator {
protected:
	const float SOUNDVEL_ = 342.2f;
	float k_, r_, t_, totalTime_;
	Vector3 origin_;

public:
	ExplosionForceGenerator(float k, float r, float t, Vector3 origin) : ForceGenerator(), k_(k), r_(r), t_(t), origin_(origin), totalTime_(0) { };
	
	inline void setK(const float& k) { k_ = k; }
	inline float getK() const { return k_; }
	inline void resetTime() { totalTime_ = 0; }

	virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			totalTime_ += t;
			auto pos = p->getPos();
			auto dist = pos - origin_;
			auto rad = dist.magnitude();
			r_ = SOUNDVEL_ * totalTime_;
			if ( rad < r_) {
				float intensity = k_ / (r_ * r_);
				float eN = exp(-t / t_);	// exp(n) = e^n

				auto f = intensity * dist * eN;
				p->addForce(f);
			}

		}
	};
};