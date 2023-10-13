#pragma once

#include "PxPhysics.h"
#include "../core.hpp"
#include "../RenderUtils.hpp"


class Particle {
public:
	// Tamaño, forma y color
	struct visual {
		float size = 1.0f;
		physx::PxGeometry* geometry;
		Vector4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
	};

	struct physics {
		Vector3 pos,	// Posición
			vel,		// Dirección/velocidad (vector)
			acc;		// Aceleración (vector) en m/s^2
		float damp,		// Rozamiento (magnitud)
			mass,		// Masa (magnitud) en kg
			simSpd;
	};


private:
	physx::PxTransform* tr_;		// Transform de la esfera (Su posición se va actualizando)
	RenderItem* renderItem_;		// Objeto renderizable

	Vector3 vel;					// Velocidad (la que se va actualizando)
	visual vis_;
	physics phys_;

	float maxLifetime_;
	float lifetime_;
	bool alive_;



public:
	Particle(visual visuals, physics phys, float maxLifetime = 1.0f);
	~Particle();

	void update(double t);

	inline void setInitPos(Vector3 pos) { phys_.pos = pos; }
	inline Vector3 getInitPos() { return phys_.pos; }

	inline void setPos(Vector3 pos) { tr_->p = pos; }
	inline Vector3 getPos() { return tr_->p; }

	inline void setInitVel(Vector3 vel) { phys_.vel = vel; }
	inline Vector3 getInitVel() { return phys_.vel; }

	inline Vector3 getVel() { return phys_.vel; }
	inline void setVel(Vector3 vel) { phys_.vel = vel; }

	inline Vector3 getAcc() { return phys_.acc; }
	void setAcc(Vector3 acc) { phys_.acc = acc; }

	inline bool isAlive() { return alive_; }
	inline void changeLifetime(float t) { maxLifetime_ = t; }
	
	inline Particle* clone() {
		return new Particle(vis_, phys_, maxLifetime_);
	}
};

