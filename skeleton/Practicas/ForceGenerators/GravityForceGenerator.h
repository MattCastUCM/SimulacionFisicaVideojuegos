#pragma once

#include "../ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
protected:
	Vector3 gravity_;

public:
	GravityForceGenerator(const Vector3& g) : ForceGenerator(), gravity_(g) { };
	
	virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			p->addForce(gravity_ * p->getMass());
		}
	};
};