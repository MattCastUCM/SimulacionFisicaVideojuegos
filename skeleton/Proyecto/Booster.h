#pragma once

#include "../Practicas/Particles/SRigidBody.h"
using namespace physx;
#include "BoosterCallback.h"

#include <iostream>
using namespace std;
#include "../checkMemLeaks.h"

class Booster : public SRigidBody {
private:
	const float SIZE_ = 5.0f;
	const Vector4 COLOR_ = { 235 / 255.0f, 64 / 255.0f, 52 / 255.0f, 1.0f };
	const Vector3 MAT_ = { 1.0f, 1.0f, 0.0f };

	BoosterCallback* bc_;

public:
	Booster(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, std::function<void()>funct, physx::PxActor* ballActor) : SRigidBody(false, -1, gPhysics, gScene) {
		Particle::visual v;
		v.size = { SIZE_, SIZE_ / 5, SIZE_ };
		v.type = Particle::geomType::geomBox;
		v.color = COLOR_;
		v.material = gPhysics->createMaterial(MAT_.x, MAT_.y, MAT_.z);

		Particle::physics p;
		p.pos = pos;
		p.damp = 0;


		vis_ = v;
		phys_ = p;
		maxLifetime_ = -1;
		lifetime_ = 0;
		alive_ = true;

		vel_ = phys_.vel;
		acc_ = phys_.acc;
		mass_ = phys_.mass;

		accumForce_ = { 0, 0, 0 };

		shape_ = makeShape();

		shape_->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape_->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		shape_->setFlag(PxShapeFlag::eVISUALIZATION, false);

		tr_ = new physx::PxTransform(phys_.pos);
		rigid_ = gPhysics_->createRigidStatic(*tr_);
		rigidActor_ = rigid_;
		rigidActor_->attachShape(*shape_);
		gScene_->addActor(*rigidActor_);
		renderItem_ = new RenderItem(shape_, rigidActor_, vis_.color);


		bc_ = new BoosterCallback(funct, ballActor);
		gScene->setSimulationEventCallback(bc_);
	};

	~Booster() {
		delete bc_;
	}
};

