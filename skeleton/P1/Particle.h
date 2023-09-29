#pragma once

#include "PxPhysics.h"
#include "../core.hpp"
#include "../RenderUtils.hpp"

class Particle {
private:
	const float SIZE_ = 2.0f;		// Tamaño de la esfera
	const Vector4 COLOR_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);		// Color de la esfera
	

	physx::PxTransform* tr_;		// Transform de la esfera
	RenderItem* renderItem_;		// Objeto renderizable

	float mass_;					// Masa (magnitud)
	float spd_;						// Velocidad/rapidez (magnitud, no vector)
	float damp_;					// Rozamiento (vector)

	Vector3 vel_;					// Dirección/velocidad (vector)
	Vector3 acc_;					// Aceleración (vector)

public:
	Particle(Vector3 pos, float mass, Vector3 vel, float spd, Vector3 acc, float damp) 
		: mass_(mass), vel_(vel), spd_(spd), acc_(acc), damp_(damp)
	{
		tr_ = new physx::PxTransform(pos);
		renderItem_ = new RenderItem(CreateShape(physx::PxSphereGeometry(2.0f)), tr_, COLOR_);

		float simVel = vel_.magnitude() / 5.0f;
		float coef = spd_ / simVel;
		float simMass = mass_ * coef * coef;

		spd_ /= 5.0f;
	}

	~Particle() {
		DeregisterRenderItem(renderItem_);
		if (tr_ != nullptr) delete tr_;
	}

	void update(double t) {
		// MRU	(no haría falta normalizar la velocidad)
		//tr_->p += SPD_ * vel_/*.getNormalized()*/ * t;


		//MRUA	(v * t + 1/2 acc * t * t) <- NO FUNCIONA porque la velocidad no se actualiza :[
		tr_->p += vel_ * spd_ * t;	// Actualizar pos
		vel_ += acc_ * t;			// Actualizar vel según acc
		vel_ *= pow(damp_, t);		// Actualizar vel según damp
	}
};

