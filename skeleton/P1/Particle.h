#pragma once

#include "PxPhysics.h"
#include "../core.hpp"
#include "../RenderUtils.hpp"


class Particle {
public:
	struct visual {
		float size;
		const physx::PxGeometry& geometry;
		Vector4 color;

		visual();
	};
	struct physics {
		Vector3 pos,
			vel,	// Dirección/velocidad (vector)
			acc;	// Aceleración (vector) en m/s^2
		float damp,		// Rozamiento (magnitud)
			mass,		// Masa (magnitud) en kg
			simSpd;

		physics();
	};


private:
	const float SIZE_ = 2.0f;		// Tamaño de la esfera

	physx::PxTransform* tr_;		// Transform de la esfera
	RenderItem* renderItem_;		// Objeto renderizable

	physics phys_;

	float maxLifetime_;
	float lifetime_;
	bool alive_;



public:
	

	Particle(visual visuals, physics phys, float maxLifetime = 5.0f);
	~Particle();

	void update(double t);

	inline void setPos(Vector3 pos) { tr_->p = pos; }
	inline Vector3 getPos() { return tr_->p; }

	inline Vector3 getVel() { return phys_.vel; }
	inline bool isAlive() { return alive_; }
	inline void changeLifetime(float t) { maxLifetime_ = t; }
	
};

