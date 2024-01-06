#include "Particle.h"
#include "../checkMemLeaks.h"

Particle::Particle(bool default, float maxLifeTime) {
	isRigid_ = false;
	if (default) {
		init(vis_, phys_, maxLifeTime);
	}
}

Particle::Particle(visual vis, physics phys, float maxLifeTime) {
	isRigid_ = false;
	init(vis, phys, maxLifeTime); 
}

Particle::~Particle() {
	if (renderItem_ != nullptr) { 
		DeregisterRenderItem(renderItem_); 
		delete renderItem_;
	}
	if (tr_ != nullptr) delete tr_;
}


void Particle::init(visual vis, physics phys, float maxLifeTime) {
	isRigid_ = false;

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
	renderItem_ = new RenderItem(shape_, tr_, vis_.color);
}

PxShape* Particle::makeShape() {
	PxShape* shape = nullptr;
	
	if (vis_.type == geomSphere)
		shape = CreateShape(PxSphereGeometry(vis_.size.x), vis_.material);
	else if (vis_.type == geomBox)
		shape = CreateShape(PxBoxGeometry(vis_.size), vis_.material);
	else
		shape = CreateShape(PxCapsuleGeometry(vis_.size.x, vis_.size.y), vis_.material);

	return shape;
}

void Particle::update(double t) {
	// Si está viva, se actualiza
	if (alive_) {
		// Si se ha pasado su tiempo de vida, se pone alive a false
		if (lifeTime_ > maxLifeTime_ && maxLifeTime_ > 0) alive_ = false;

		// Si no, se actualiza su tiempo de vida y su pos, vel y acc
		else {
			lifeTime_ += t;
			// MRU	(no haría falta normalizar la velocidad)
			//tr_->p += SPD_ * vel_/*.getNormalized()*/ * t;

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
	}
}

void Particle::clearForce() { accumForce_ *= 0; }

void Particle::addForce(const Vector3& f) { 
	accumForce_ += f; 
}


Particle* Particle::clone() {
	return new Particle(vis_, phys_, maxLifeTime_);
}
