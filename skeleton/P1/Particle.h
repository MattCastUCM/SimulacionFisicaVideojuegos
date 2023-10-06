#pragma once

#include "PxPhysics.h"
#include "../core.hpp"
#include "../RenderUtils.hpp"


class Particle {
public:
	struct visual {
		float size;
		physx::PxGeometry& geometry;
		Vector4 color;

		visual() : size(1.0f), geometry(physx::PxSphereGeometry(size)), color({ 1.0f, 0.0f, 0.0f, 1.0f }) {	};
	};

	struct physics {
		Vector3 pos,
			vel,	// Dirección/velocidad (vector)
			acc;	// Aceleración (vector) en m/s^2
		float damp,		// Rozamiento (magnitud)
			mass,		// Masa (magnitud) en kg
			simSpd;
	};


private:
	const float SIZE_ = 2.0f;		// Tamaño de la esfera

	physx::PxTransform* tr_;		// Transform de la esfera
	RenderItem* renderItem_;		// Objeto renderizable

	visual vis_;
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
	
	inline Particle* clone() {
		return new Particle(vis_, phys_, maxLifetime_);
	}
};

