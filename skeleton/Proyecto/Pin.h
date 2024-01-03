#pragma once

#include "../Practicas/Particles/DRigidBody.h"
using namespace physx;
#include <functional>

#include <iostream>
using namespace std;

class Pin : public DRigidBody {
private:
	const float RADIUS_ = 0.75f, 
				HALFH_ = 3.8f / 2 - RADIUS_, 
				DENS_ = 0.285f,
				DELAY_ = 2.0f;
	const Vector4 COLOR_ = { 242 / 255.0f, 238 / 255.0f, 213 / 255.0f, 1.0f };
	const Vector3 MAT_ = { 1.0f, 1.0f, 0.0f };

	bool remove_;
	float removeDelay_;
	std::function<void(Pin* pin)> callback_;

public:
	Pin(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, PxU32 colGroup, PxU32 colMask, std::function<void(Pin* pin)>funct)
		: DRigidBody(false, -1, gPhysics, gScene), remove_(false), removeDelay_(0), callback_(funct) 
	{
		Particle::visual v;

		v.geometry = new physx::PxCapsuleGeometry(RADIUS_, HALFH_);
		v.color = COLOR_;
		v.material = gPhysics->createMaterial(MAT_.x, MAT_.y, MAT_.z);

		Particle::physics p;
		p.pos = pos;
		p.damp = 0;
		// Se guarda la densidad. De media, la masa de un bolo es de 1.66 kg
		p.mass = DENS_;
		p.colGrp = colGroup;
		p.colMask = colMask;

		init(v, p, -1);

		PxTransform rotatedTr(rigid_->getGlobalPose().p, PxQuat(PxHalfPi, Vector3(0, 0, -1)));
		rigid_->setGlobalPose(rotatedTr);
	}


	inline virtual void update(double t) {
		DRigidBody::update(t);
		
		auto rot = tr_->q;
		cout << rot.x << ' ' << rot.y << ' ' << rot.z << '\n';
		if ((rot.x >= 0.5f || rot.x <= -0.5f /*|| rot.z >= 0.9f || rot.z <= -0.9f*/))
			remove_ = true;

		if(remove_){
			removeDelay_ += t;
			if (removeDelay_ >= DELAY_) callback_(this);
		}
	};
};