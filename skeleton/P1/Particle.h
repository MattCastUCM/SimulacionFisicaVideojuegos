#pragma once

#include "PxPhysics.h"
#include "../core.hpp"
#include "../RenderUtils.hpp"

class Particle {
private:
	const float SIZE_ = 2.0f;		// Tamaño de la esfera
	const Vector4 COLOR_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);		// Color de la esfera
	const float SPD_ = 3.0f;			// Velocidad/rapidez (magnitud, no vector)


	physx::PxTransform* tr_;		// Transform de la esfera
	RenderItem* renderItem_;		// Objeto renderizable
	Vector3 vel_;					// Dirección/velocidad (vector)
	Vector3 acc_;					// Aceleración (vector)

public:
	Particle(Vector3 pos) {
		tr_ = new physx::PxTransform(pos);
		renderItem_ = new RenderItem(CreateShape(physx::PxSphereGeometry(2.0f)), tr_, COLOR_);


		// Puestos a la fuerza, se podrían cambiar pulsando teclas
		vel_ = Vector3(0.0f, 1.0f, 0.0f);
		acc_ = Vector3(0.0f, 4.0f, 0.0f);
	}

	~Particle() {
		DeregisterRenderItem(renderItem_);
		if (tr_ != nullptr) delete tr_;
	}

	void update(double t) {
		// MRU	(no haría falta normalizar la velocidad)
		//tr_->p += SPD_ * vel_/*.getNormalized()*/ * t;

		//MRUA	(v * t + 1/2 acc * t * t) NO FUNCIONA :[
		vel_ += acc_ * t;
		tr_->p += vel_ * t;
	}
};

