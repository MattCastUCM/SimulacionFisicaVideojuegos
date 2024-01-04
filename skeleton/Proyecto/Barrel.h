#pragma once

#include "../Practicas/Particles/DRigidBody.h"
using namespace physx;
#include "../checkMemLeaks.h"

class Barrel : public DRigidBody {
private:
	const float W_ = 1.0f, H_ = 2.0f,
				DENS_ = 0.285f;

	const Vector4 COLOR_ = { 1.0f, 0.0f, 0.0f, 1.0f };
	const Vector3 MAT_ = { 1.0f, 1.0f, 0.0f };

public:
	Barrel(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos) : DRigidBody(false, -1, gPhysics, gScene) {
		Particle::visual v;

		v.geometry = new physx::PxBoxGeometry(W_, H_, W_);
		v.color = COLOR_;
		v.material = gPhysics->createMaterial(MAT_.x, MAT_.y, MAT_.z);

		Particle::physics p;
		p.pos = pos;
		p.damp = 0;
		// Se guarda la densidad. De media, la masa de un bolo es de 1.66 kg
		p.mass = DENS_;

		init(v, p, -1);
	}

};