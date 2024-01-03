#pragma once

#include "../Practicas/ForceGenerators/SpringForceGenerator.h"
#include "../Practicas/Particles/DRigidBody.h"

#include <iostream>
using namespace std;
class RigidSpringForceGenerator : public SpringForceGenerator {
public:
	RigidSpringForceGenerator(double k, double restingLength, Particle* other, bool autoInactive = false) : SpringForceGenerator(k, restingLength, other, autoInactive)
	{ }

	virtual void update(Particle* p, double t) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			Vector3 posDif = other_->getPos() - p->getPos();
			float delta = posDif.normalize() - restingLength_;
			Vector3 f = posDif * delta * k_;

			((DRigidBody*)p)->addForce(f);
			//cout << f.x << ' ' << f.y << ' ' << f.z << '\n';
		}

	};
};