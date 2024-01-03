#include "DRigidBody.h"

DRigidBody::DRigidBody(bool default, float maxLifetime, PxPhysics* gPhys, PxScene* gScene) {
	rigidActor_ = nullptr;
	rigid_ = nullptr;
	shape_ = nullptr;
	gPhysics_ = gPhys;
	gScene_ = gScene;
	if (default) {
		vis_.geometry = new physx::PxSphereGeometry(vis_.size);
		init(vis_, phys_, maxLifetime);
	}
}

DRigidBody::DRigidBody(visual vis, physics phys, float maxLifetime, PxPhysics* gPhys, PxScene* gScene) {
	rigidActor_ = nullptr;
	rigid_ = nullptr;
	shape_ = nullptr;
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

	shape_ = CreateShape(*vis_.geometry, vis_.material);
	tr_ = new physx::PxTransform(phys_.pos);

	// MATRIZ DE COLISION (SE TIENE QUE CREAR ANTES DE CREAR EL RIGIDO)
	shape_->setSimulationFilterData(PxFilterData(phys_.colGrp, phys_.colMask, 0, 0));

	rigid_ = gPhysics_->createRigidDynamic(*tr_);
	rigidActor_ = rigid_;
	rigidActor_->attachShape(*shape_);
	gScene_->addActor(*rigidActor_);
	renderItem_ = new RenderItem(shape_, rigidActor_, vis_.color);

	// ASUMIENDO QUE EN mass_ SE GUARDA la densidad
	PxRigidBodyExt::updateMassAndInertia(*rigid_, mass_);		// NO USAR rigid_->setMass(1 / mass_);
	rigid_->setLinearVelocity(vel_);
	rigid_->setAngularVelocity(vel_);


	// Usar en otros casos?
	//rigid_->setLinearDamping(phys_.damp);
	//rigid_->setAngularVelocity();
	//rigid_->setAngularDamping();	
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
			vel_ = rigid_->getLinearVelocity();
		}
	}
}
void DRigidBody::addForce(const Vector3& f, PxForceMode::Enum mode, bool autoawake) {
	rigid_->addForce(f, mode, autoawake);
}
//void DRigidBody::addTorque(const Vector3& t, PxForceMode::Enum mode, bool autoawake) {
//	rigid_->addTorque(t, mode, autoawake);
//}


Particle* DRigidBody::clone() {
	DRigidBody::visual v;
	v.size = vis_.size;
	v.geometry = vis_.geometry;
	v.color = vis_.color;

	return new DRigidBody(v, phys_, maxLifetime_, gPhysics_, gScene_);
}
