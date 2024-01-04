#pragma once

#include "../Practicas/ForceGenerators/SpringForceGenerator.h"
#include "../checkMemLeaks.h"

class RotatedSpringForceGenerator : public SpringForceGenerator {
protected:
	Vector3 f_;

public:
	RotatedSpringForceGenerator(double k, double restingLength, Particle* other, bool autoInactive = false) : SpringForceGenerator(k, restingLength, other)
	{}

	virtual void update(Particle* p, double t) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			Vector3 posDif = other_->getPos() - p->getPos();
			float delta = posDif.normalize() - restingLength_;
			
			f_ = posDif * delta * k_;
			p->addForce(f_);
		}
		
	};

	inline Vector3 getForce() const { return f_; }
};