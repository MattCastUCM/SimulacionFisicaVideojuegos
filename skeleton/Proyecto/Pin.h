#pragma once

#include "../Practicas/Particles/DRigidBody.h"
#include <iostream>
using namespace std;
using namespace physx;

class Pin : public DRigidBody {
private:
	float rad = 0.0516f, halfH = 0.38f / 2 - 0.0516f, dens = 575;

public:
	Pin(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos) : DRigidBody(false, -1, gPhysics, gScene) {
		Particle::visual v;

		v.geometry = new physx::PxCapsuleGeometry(rad, halfH);
		v.color = { 242 / 255.0f, 238 / 255.0f, 213 / 255.0f, 1.0f };

		Particle::physics p;
		p.pos = pos;
		p.damp = 0;
		p.mass = dens;	// Se guarda la densidad. De media, la masa de un bolo es de 1.66 kg



		vis_ = v;
		phys_ = p;
		maxLifetime_ = -1;
		lifetime_ = 0;
		alive_ = true;

		vel_ = phys_.vel;
		acc_ = phys_.acc;
		mass_ = phys_.mass;

		accumForce_ = { 0, 0, 0 };

		shape_ = CreateShape(*vis_.geometry, vis_.material);
		PxTransform relativePose(PxQuat(PxHalfPi, Vector3(0, 0, 1)));
		shape_->setLocalPose(relativePose);

		tr_ = new physx::PxTransform(phys_.pos);
		rigid_ = gPhysics_->createRigidDynamic(*tr_);
		rigidActor_ = rigid_;
		rigidActor_->attachShape(*shape_);
		gScene_->addActor(*rigidActor_);
		renderItem_ = new RenderItem(shape_, rigidActor_, vis_.color);

		// ASUMIENDO QUE EN mass_ SE GUARDA la densidad
		PxRigidBodyExt::updateMassAndInertia(*rigid_, mass_);		// NO USAR rigid_->setMass(1 / mass_);
		rigid_->setLinearVelocity(vel_);
		rigid_->setLinearDamping(phys_.damp);


		tr_->q.x = 1.5f;
		tr_->q.y = 0.1f;
		tr_->q.z = 0.8f;
		tr_->q.rotate(Vector3(1, 1, 1));
		//cout <<  << ' ' << tr_->q.y << ' ' << tr_->q.z << '\n';
	}

};