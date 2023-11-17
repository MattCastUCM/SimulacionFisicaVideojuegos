#pragma once

#include "../ForceGenerator.h"

class SpringForceGenerator : public ForceGenerator {
protected:
	double k_, restingLength_;
	Particle* other_;

public:
	SpringForceGenerator(double k, double restingLength, Particle* other) : ForceGenerator(), k_(k), restingLength_(restingLength), other_(other) { };
	
	inline void setK(double k) { k_ = k; }
	inline double getK() { return k_; }

	virtual void update(Particle* p, double t) { 
		Vector3 posDif = other_->getPos() - p->getPos();
		float delta = posDif.normalize() - restingLength_;
		Vector3 f = posDif * delta * k_;

		p->addForce(f);
	};
};