#pragma once

#include "../Practicas/ForceGenerator.h"
#include "../Practicas/Particles/DRigidBody.h"

class HolePushForceGenerator : public ForceGenerator {
public:
	HolePushForceGenerator(double maxLifetime = -1) : ForceGenerator("", maxLifetime) {

	};

	virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			// Aplica una fuerza de impulso hacia el fondo
			Vector3 dir = { 0, 0, -1 };

			((DRigidBody*)p)->addForce(dir * 0.4, physx::PxForceMode::eIMPULSE);
		}
	};
};