#pragma once

#include "../Practicas/ForceGenerator.h"
#include "../Practicas/Particles/DRigidBody.h"
#include "../checkMemLeaks.h"

class ImpulseForceGenerator : public ForceGenerator {
protected:
	float k_;
	Vector3 dir_;

public:
	ImpulseForceGenerator(Vector3 dir, float k = 10) : ForceGenerator("", -1, true), dir_(dir), k_(k) {}

	inline virtual void update(Particle* p, double t) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {

			auto f = dir_ * k_;
			((DRigidBody*)p)->addForce(f, physx::PxForceMode::eIMPULSE);
		}
	};

	inline void setDir(Vector3 d) { dir_ = d; }
};