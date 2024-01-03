#pragma once

#include "../Practicas/Particles/DRigidBody.h"
#include <iostream>
using namespace std;
using namespace physx;

class Pin : public DRigidBody {
private:
	const float RADIUS_ = 0.75f, 
				HALFH_ = 3.8f / 2 - RADIUS_, 
				DENS_ = 0.285f;

public:
	Pin(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos) : DRigidBody(false, -1, gPhysics, gScene) {
		Particle::visual v;

		v.geometry = new physx::PxCapsuleGeometry(RADIUS_, HALFH_);
		v.color = { 242 / 255.0f, 238 / 255.0f, 213 / 255.0f, 1.0f };

		Particle::physics p;
		p.pos = pos;
		p.damp = 0;
		// Se guarda la densidad. De media, la masa de un bolo es de 1.66 kg
		p.mass = DENS_;

		init(v, p, -1);

		PxTransform rotatedTr(rigid_->getGlobalPose().p, PxQuat(PxHalfPi, Vector3(0, 0, -1)));
		rigid_->setGlobalPose(rotatedTr);
	}


};