#include "Particle.h"

Particle::Particle(visual visuals, physics phys, float maxLifetime)
	: vis_(visuals), phys_(phys), maxLifetime_(maxLifetime), lifetime_(0), alive_(true)
{
	tr_ = new physx::PxTransform(phys.pos);
	renderItem_ = new RenderItem(CreateShape(*vis_.geometry), tr_, visuals.color);

	// Masa simulada
	float coefMass = phys.vel.magnitude() / phys.simSpd;
	float simMass = phys_.mass * coefMass * coefMass;
	// Aceleraci�n simulada
	float coefAcc = phys.simSpd / phys.vel.magnitude();
	Vector3 simAcc = phys_.acc / (coefAcc * coefAcc);

	// Hace que la velocidad y aceleraci�n pasen a ser las simuladas
	phys_.vel /= coefMass;
	phys_.mass = simMass;
	phys_.acc = simAcc;

	vel = phys_.vel;
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem_);
	if (tr_ != nullptr) delete tr_;
}


#include <iostream>
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
			vel += phys_.acc * t;			// Actualizar vel seg�n acc
			vel *= pow(phys_.damp, t);		// Actualizar vel seg�n damp
			tr_->p += vel * t;			// Actualizar pos

			std::cout << this << ' ' << tr_->p.x << ' ' << tr_->p.y << ' ' << tr_->p.z << '\n';
		}
	}
}