#include "DRigidBody.h"

DRigidBody::DRigidBody(bool default, float maxLifetime, PxPhysics* gPhys, PxScene* gScene) {
	rigidActor_ = nullptr;
	rigid_ = nullptr;
	gPhysics_ = gPhys;
	gScene_ = gScene;
	if (default) {
		DRigidBody::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxSphereGeometry(v.size);

		v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		DRigidBody::physics p;
		p.damp = 0.998f;
		p.pos = { 0, 0, 0 };
		p.vel = { 0, 0, 0 };
		p.acc = { 0, 0, 0 };
		p.mass = 1 / 5.4f;

		init(v, p, maxLifetime);
	}
}

DRigidBody::DRigidBody(visual vis, physics phys, float maxLifetime, PxPhysics* gPhys, PxScene* gScene) {
	rigidActor_ = nullptr;
	rigid_ = nullptr;
	gPhysics_ = gPhys;
	gScene_ = gScene;

	init(vis, phys, maxLifetime);
}

DRigidBody::~DRigidBody() {
	gScene_->removeActor(*rigidActor_);
}


void DRigidBody::init(visual vis, physics phys, float maxLifetime) {
	vis_ = vis;
	phys_ = phys;
	maxLifetime_ = maxLifetime;
	lifetime_ = 0;
	alive_ = true;

	vel_ = phys_.vel;
	acc_ = phys_.acc;
	mass_ = phys_.mass;

	accumForce_ = { 0, 0, 0 };

	PxShape* shape = CreateShape(*vis_.geometry);
	tr_ = new physx::PxTransform(phys_.pos);
	rigid_ = gPhysics_->createRigidDynamic(*tr_);
	rigidActor_ = rigid_;
	rigidActor_->attachShape(*shape);
	gScene_->addActor(*rigidActor_);
	renderItem_ = new RenderItem(shape, rigidActor_, vis_.color);

	// ASUMIENDO QUE EN mass_ SE GUARDA la densidad
	PxRigidBodyExt::updateMassAndInertia(*rigid_, mass_);		// NO USAR rigid_->setMass(1 / mass_);
	rigid_->setLinearVelocity(vel_);
	rigid_->setLinearDamping(phys_.damp);

	// Usar en otros casos?
	//rigid_->setAngularVelocity();
	//rigid_->setAngularDamping();
	//rigid_->setMassSpaceInertiaTensor();

	
}

void DRigidBody::update(double t) {
	// Si está viva, se actualiza
	if (alive_) {
		// Si se ha pasado su tiempo de vida, se pone alive a false
		if (lifetime_ > maxLifetime_ && maxLifetime_ > 0) alive_ = false;

		// Si no, se actualiza su tiempo de vida y su pos, vel y acc
		else {
			lifetime_ += t;
			*tr_ = rigid_->getGlobalPose();
		}
	}
}
void DRigidBody::addForce(const Vector3& f) {
	rigid_->addForce(f);
	//rigid_->addTorque
}


Particle* DRigidBody::clone() {
	DRigidBody::visual v;
	v.size = vis_.size;
	v.geometry = vis_.geometry;
	v.color = vis_.color;

	return new DRigidBody(v, phys_, maxLifetime_, gPhysics_, gScene_);
}
