#include "Particle.h"
#include "../checkMemLeaks.h"

Particle::Particle(bool default, float maxLifetime) {
	if (default) {
		vis_.geometry = new physx::PxSphereGeometry(vis_.size);
		init(vis_, phys_, maxLifetime);
	}
}

Particle::Particle(visual vis, physics phys, float maxLifetime) {
	init(vis, phys, maxLifetime); 
}

Particle::~Particle() {
	if (renderItem_ != nullptr) { 
		DeregisterRenderItem(renderItem_); 
		delete renderItem_;
	}
	if (tr_ != nullptr) delete tr_;
	if (!vis_.clone && vis_.geometry != nullptr) delete vis_.geometry;
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

	PxShape* shape = CreateShape(*vis_.geometry, vis_.material);
	tr_ = new physx::PxTransform(phys_.pos);
	renderItem_ = new RenderItem(shape, tr_, vis_.color);

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
	Particle::visual v;
	v.size = vis_.size;
	v.geometry = vis_.geometry;
	v.color = vis_.color;
	v.clone = true;

	return new Particle(v, phys_, maxLifetime_);
}
