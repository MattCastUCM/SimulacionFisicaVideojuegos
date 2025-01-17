#include "SRigidBody.h"
#include "../../checkMemLeaks.h"

SRigidBody::SRigidBody(bool default, float maxLifeTime, PxPhysics* gPhys, PxScene* gScene) : Particle() {
	isRigid_ = true;
	
	rigidActor_ = nullptr;
	rigid_ = nullptr;
	shape_ = nullptr;
	gPhysics_ = gPhys;
	gScene_ = gScene;
}

SRigidBody::SRigidBody(visual vis, physics phys, float maxLifeTime, PxPhysics* gPhys, PxScene* gScene) : Particle() {
	isRigid_ = true;
	
	rigidActor_ = nullptr;
	rigid_ = nullptr;
	shape_ = nullptr;
	gPhysics_ = gPhys;
	gScene_ = gScene;

	init(vis, phys, maxLifeTime);
}

SRigidBody::~SRigidBody() {
	gScene_->removeActor(*rigidActor_);
}


void SRigidBody::init(visual vis, physics phys, float maxLifeTime) {
	isRigid_ = true;
	
	vis_ = vis;
	phys_ = phys;
	maxLifeTime_ = maxLifeTime;
	lifeTime_ = 0;
	alive_ = true;

	vel_ = phys_.vel;
	acc_ = phys_.acc;
	mass_ = phys_.mass;

	accumForce_ = { 0, 0, 0 };

	shape_ = makeShape();
	tr_ = new physx::PxTransform(phys_.pos);

	rigid_ = gPhysics_->createRigidStatic(*tr_);
	rigidActor_ = rigid_;
	rigidActor_->attachShape(*shape_);
	gScene_->addActor(*rigidActor_);


	renderItem_ = new RenderItem(shape_, rigidActor_, vis_.color);
}

void SRigidBody::update(double t) {
	// Si est� viva, se actualiza
	if (alive_) {
		// Si se ha pasado su tiempo de vida, se pone alive a false
		if (lifeTime_ > maxLifeTime_ && maxLifeTime_ > 0) alive_ = false;

		// Si no, se actualiza su tiempo de vida y su pos, vel y acc
		else {
			lifeTime_ += t;
			*tr_ = rigid_->getGlobalPose();
		}
	}
}


Particle* SRigidBody::clone() {
	return new SRigidBody(vis_, phys_, maxLifeTime_, gPhysics_, gScene_);
}
