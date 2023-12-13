#include "Particle.h"

Particle::Particle(bool default, float maxLifetime, PxPhysics* gPhys, PxScene* gScene, bool dynamic) {
	rigidActor_ = nullptr;
	rigidDynamic_ = nullptr;
	rigidStatic_ = nullptr;
	if (gPhys != nullptr) {
		gPhysics_ = gPhys;
		gScene_ = gScene;
		dynamic_ = dynamic;
	}
	if (default) {
		Particle::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxSphereGeometry(v.size);
		
		v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0, 0, 0 };
		p.vel = { 0, 0, 0 };
		p.acc = { 0, 0, 0 };
		p.mass = 1 / 5.4f;

		init(v, p, maxLifetime);
	}
}

Particle::Particle(visual vis, physics phys, float maxLifetime, PxPhysics* gPhys, PxScene* gScene, bool dynamic) {
	rigidActor_ = nullptr;
	rigidDynamic_ = nullptr;
	rigidStatic_ = nullptr;
	if (gPhys != nullptr) {
		gPhysics_ = gPhys;
		gScene_ = gScene;
		dynamic_ = dynamic;
	}
	init(vis, phys, maxLifetime); 
}

Particle::~Particle() {
	if (gPhysics_ != nullptr) {
		DeregisterRenderItem(renderItem_);
		if (tr_ != nullptr) delete tr_;
	}
	else {
		gScene_->removeActor(*rigidActor_);
	}
}


void Particle::init(visual vis, physics phys, float maxLifetime) {
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
	if (gPhysics_ == nullptr) {
		renderItem_ = new RenderItem(shape, tr_, vis_.color);
	}
	else {
		if (!dynamic_) {
			rigidStatic_ = gPhysics_->createRigidStatic(*tr_);
			rigidActor_ = rigidStatic_;
			rigidActor_->attachShape(*shape);
			gScene_->addActor(*rigidActor_);
			renderItem_ = new RenderItem(shape, rigidActor_, vis_.color);
		}
		else {
			rigidDynamic_ = gPhysics_->createRigidDynamic(*tr_);
			rigidActor_ = rigidDynamic_;
			rigidActor_->attachShape(*shape);
			gScene_->addActor(*rigidActor_);
			renderItem_ = new RenderItem(shape, rigidActor_, vis_.color);
			
			rigidDynamic_->setMass(1 / mass_);
			rigidDynamic_->setLinearVelocity(vel_);
			rigidDynamic_->setLinearDamping(phys_.damp);
			
			//rigidDynamic_->setAngularVelocity();
			//rigidDynamic_->setAngularDamping();
			//rigidDynamic_->setMassSpaceInertiaTensor();
			
			//PxRigidBodyExt::updateMassAndInertia(*rigidActor_)
		}
	}
}

void Particle::update(double t) {
	// Si está viva, se actualiza
	if (alive_) {
		// Si se ha pasado su tiempo de vida, se pone alive a false
		if (lifetime_ > maxLifetime_ && maxLifetime_ > 0) alive_ = false;

		// Si no, se actualiza su tiempo de vida y su pos, vel y acc
		else {
			lifetime_ += t;
			// MRU	(no haría falta normalizar la velocidad)
			//tr_->p += SPD_ * vel_/*.getNormalized()*/ * t;

			if (gPhysics_ == nullptr) {
				// Fuerzas (acc = F / m), mass_ guarda la masa inversa
				Vector3 resultaccel = accumForce_ * mass_;
				vel_ += resultaccel * t;

				//MRUA	
				// (v * t + 1/2 acc * t * t) <- NO FUNCIONA porque la velocidad no se actualiza :[
				//vel_ += acc_ * t;				// Actualizar vel según acc
				vel_ *= pow(phys_.damp, t);		// Actualizar vel según damp
				tr_->p += vel_ * t;				// Actualizar pos

			
				// Quita la fuerza acumulada
				clearForce();
			}
			else {
				if (dynamic_) *tr_ = rigidDynamic_->getGlobalPose();
				else *tr_ = rigidStatic_->getGlobalPose();
			}
		}
	}
}

void Particle::clearForce() { accumForce_ *= 0; }

void Particle::addForce(const Vector3& f) { 
	accumForce_ += f; 
	if (dynamic_) {
		rigidDynamic_->addForce(f);
		//rigidDynamic_->addTorque

	}
}


Particle* Particle::clone() {
	Particle::visual v;
	v.size = vis_.size;
	v.geometry = vis_.geometry;
	v.color = vis_.color;

	return new Particle(v, phys_, maxLifetime_, gPhysics_, gScene_, dynamic_);
}
