#pragma once

#include "../Practicas/ForceGenerators/SpringForceGenerator.h"
#include "../Practicas/Particles/DRigidBody.h"

#include <iostream>
class RigidSpringForceGenerator : public SpringForceGenerator {
public:
	RigidSpringForceGenerator(double k, double restingLength, Particle* other, bool autoInactive = false) : SpringForceGenerator(k, restingLength, other, autoInactive)
	{}

	virtual void update(Particle* p, double t) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			Vector3 posDif = other_->getPos() - p->getPos();
			//std::cout << posDif.y << '\n';

			//para que no se pare justo cuando están en la misma posición
			//delta -= p->getSize() / 2;
			float delta = posDif.normalize() - restingLength_;
			//std::cout << restingLength_ << ' ' << delta << '\n';

			Vector3 f = posDif * delta * k_;
			((DRigidBody*)p)->addForce(f/*, physx::PxForceMode::eIMPULSE*/);
		}

	};
};