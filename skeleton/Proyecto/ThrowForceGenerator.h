#pragma once

#include "../Practicas/ForceGenerator.h"
#include "../Practicas/Particles/DRigidBody.h"

class ThrowForceGenerator : public ForceGenerator {
protected:
	float initSpd_;

public:
	// Por defecto la velocidad inicial con la que se lanzará la bola serán 8.95m/s
	// (la velocidad media a la que se lanza una bola de bolos)
	ThrowForceGenerator(float initSpd = 8.95f, double maxLifetime = -1) : ForceGenerator("", maxLifetime), initSpd_(initSpd) {

	};

	virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			// Se puede estimar la fuerza a partir de la velocidad inicial, ya que solo
			// se aplica en el momento del lanzamiento => F = m * v
			float k = ((DRigidBody*)p)->getMass() * initSpd_;

			// Aplica una fuerza de impulso en la dirección de la cámara
			auto dir = GetCamera()->getDir();
			dir.normalize();
			dir.y = 0;

			((DRigidBody*)p)->addForce(dir * k, physx::PxForceMode::eIMPULSE);
		}
	};
};