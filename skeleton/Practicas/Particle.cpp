#include "Particle.h"

#include<iostream>
Particle::Particle(visual vis, physics phys, float maxLifetime) {
	init(vis, phys, maxLifetime);
	
}

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

	vel = phys_.vel;
	acc = phys_.acc;
	mass = phys_.mass;

	simulatePhys();
}


void Particle::simulatePhys() {
	// Masa simulada
	float coefMass = phys_.vel.magnitude() / phys_.simSpd;
	float simMass = phys_.mass * coefMass * coefMass;
	// Aceleraci�n simulada
	float coefAcc = phys_.simSpd / phys_.vel.magnitude();
	Vector3 simAcc = phys_.acc / (coefAcc * coefAcc);

	// Hace que la velocidad y aceleraci�n pasen a ser las simuladas
	vel /= coefMass;
	mass = simMass;
	acc = simAcc;
}



void Particle::update(double t) {
	// Si est� viva, se actualiza
	if (alive_) {
		// Si se ha pasado su tiempo de vida, se pone alive a false
		if (lifetime_ > maxLifetime_) alive_ = false;

		// Si no, se actualiza su tiempo de vida y su pos, vel y acc
		else {
			lifetime_ += t;
			// MRU	(no har�a falta normalizar la velocidad)
			//tr_->p += SPD_ * vel_/*.getNormalized()*/ * t;

			//MRUA	
			// (v * t + 1/2 acc * t * t) <- NO FUNCIONA porque la velocidad no se actualiza :[
			vel += acc * t;					// Actualizar vel seg�n acc
			vel *= pow(phys_.damp, t);		// Actualizar vel seg�n damp
			tr_->p += vel * t;				// Actualizar pos

		}
	}
}


Particle* Particle::clone() {
	Particle::visual v;
	v.size = vis_.size;
	v.geometry = vis_.geometry;
	v.color = vis_.color;

	return new Particle(v, phys_, maxLifetime_);
}