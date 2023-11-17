#include "Particle.h"

Particle::Particle(bool default, float maxLifetime) {
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

Particle::Particle(visual vis, physics phys, float maxLifetime) { init(vis, phys, maxLifetime); }

Particle::~Particle() {
	DeregisterRenderItem(renderItem_);
	if (tr_ != nullptr) delete tr_;
}


void Particle::init(visual vis, physics phys, float maxLifetime) {
	vis_ = vis;
	phys_ = phys;
	maxLifetime_ = maxLifetime;
	lifetime_ = 0;
	alive_ = true;

	tr_ = new physx::PxTransform(phys_.pos);
	renderItem_ = new RenderItem(CreateShape(*vis_.geometry), tr_, vis_.color);

	vel_ = phys_.vel;
	acc_ = phys_.acc;
	mass_ = phys_.mass;

	accumForce_ = { 0, 0, 0 };
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
			vel_ += acc_ * t;					// Actualizar vel según acc
			vel_ *= pow(phys_.damp, t);		// Actualizar vel según damp
			tr_->p += vel_ * t;				// Actualizar pos

			
			// Quita la fuerza acumulada
			clearForce();
		}
	}
}

void Particle::clearForce() { accumForce_ *= 0; }

void Particle::addForce(const Vector3& f) { accumForce_ += f; }


Particle* Particle::clone() {
	Particle::visual v;
	v.size = vis_.size;
	v.geometry = vis_.geometry;
	v.color = vis_.color;

	return new Particle(v, phys_, maxLifetime_);
}
