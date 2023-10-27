#pragma once

#include "../Particle.h"

class Projectile : public Particle {
public:
	Projectile(Particle::visual v, Particle::physics p, float simSpd, const Vector3& g = { 0, 0, 0}, double lifetime = 1.0f) 
		: Particle(v, p, lifetime)
	{
		// Masa simulada
		float coefMass = phys_.vel.magnitude() / simSpd;
		float simMass = phys_.mass * coefMass * coefMass;
		// Aceleración simulada
		float coefAcc = simSpd / phys_.vel.magnitude();
		Vector3 simAcc = phys_.acc / (coefAcc * coefAcc);

		// Hace que la velocidad y aceleración pasen a ser las simuladas
		vel_ /= coefMass;
		mass_ = simMass;
		acc_ = simAcc;
	}

};