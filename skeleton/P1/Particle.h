#pragma once

#include "PxPhysics.h"
#include "../core.hpp"
#include "../RenderUtils.hpp"

class Particle {
private:
	const float SIZE_ = 2.0f;		// Tamaño de la esfera
	const float MAXLIFETIME_ = 2.0f;

	physx::PxTransform* tr_;		// Transform de la esfera
	RenderItem* renderItem_;		// Objeto renderizable

	Vector3 vel_;					// Dirección/velocidad (vector)
	float spd_;						// Velocidad/rapidez (magnitud, no vector) en m/s

	Vector3 acc_;					// Aceleración (vector) en m/s^2
	float damp_;					// Rozamiento (vector)

	float mass_;					// Masa (magnitud) en kg

	float lifetime_;
	bool alive_;



public:
	Particle(float size, Vector3 pos, Vector3 vel, float spd, Vector3 acc, float damp, float mass, Vector4 color, float simSpd = 1)
		:  vel_(vel), spd_(spd), acc_(acc), damp_(damp), mass_(mass), lifetime_(0), alive_(true)
	{
		tr_ = new physx::PxTransform(pos);
		renderItem_ = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), tr_, color);

		// Masa simulada
		float coefMass = spd_ / simSpd;
		float simMass = mass_ * coefMass * coefMass;

		// Aceleración simulada
		float coefAcc = simSpd / spd_;
		Vector3 simAcc = acc_ * coefAcc * coefAcc;

		// Hace que la velocidad y aceleración pasen a ser las simuladas
		spd_ = simSpd;
		acc_ = simAcc;
	}

	~Particle() {
		DeregisterRenderItem(renderItem_);
		if (tr_ != nullptr) delete tr_;
	}


	void update(double t) {
		if (alive_) {
			if (lifetime_ > MAXLIFETIME_) alive_ = false;
			else {
				lifetime_ += t;
				// MRU	(no haría falta normalizar la velocidad)
				//tr_->p += SPD_ * vel_/*.getNormalized()*/ * t;

				//MRUA	
				// (v * t + 1/2 acc * t * t) <- NO FUNCIONA porque la velocidad no se actualiza :[
				tr_->p += vel_ * spd_ * t;	// Actualizar pos
				vel_ += acc_ * t;			// Actualizar vel según acc
				vel_ *= pow(damp_, t);		// Actualizar vel según damp
			}
		}
	}

	bool isAlive() { return alive_; }
};

