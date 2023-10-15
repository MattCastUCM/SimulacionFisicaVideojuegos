#include "Particle.h"


Particle::Particle() { }

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

	tr_ = new physx::PxTransform(phys.pos);
	renderItem_ = new RenderItem(CreateShape(*vis.geometry), tr_, vis.color);

	vel = phys.vel;
	acc = phys.acc;
	mass = phys.mass;

	// Masa simulada
	float coefMass = phys.vel.magnitude() / phys.simSpd;
	float simMass = phys.mass * coefMass * coefMass;
	// Aceleración simulada
	float coefAcc = phys.simSpd / phys.vel.magnitude();
	Vector3 simAcc = phys.acc / (coefAcc * coefAcc);

	// Hace que la velocidad y aceleración pasen a ser las simuladas
	vel /= coefMass;
	mass = simMass;
	acc = simAcc;
}



void Particle::update(double t) {
	// Si está viva, se actualiza
	if (alive_) {
		// Si se ha pasado su tiempo de vida, se pone alive a false
		if (lifetime_ > maxLifetime_) alive_ = false;

		// Si no, se actualiza su tiempo de vida y su pos, vel y acc
		else {
			lifetime_ += t;
			// MRU	(no haría falta normalizar la velocidad)
			//tr_->p += SPD_ * vel_/*.getNormalized()*/ * t;

			//MRUA	
			// (v * t + 1/2 acc * t * t) <- NO FUNCIONA porque la velocidad no se actualiza :[
			vel += acc * t;					// Actualizar vel según acc
			vel *= pow(phys_.damp, t);		// Actualizar vel según damp
			tr_->p += vel * t;				// Actualizar pos

		}
	}
}
