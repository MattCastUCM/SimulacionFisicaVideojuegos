#pragma once

#include "PxPhysics.h"
#include "../core.hpp"
#include "../RenderUtils.hpp"

class Particle {
public:
	// Tama�o, forma y color
	struct visual {
		float size = 1.0f;
		physx::PxGeometry* geometry;
		Vector4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
	};

	// F�sicas iniciales
	struct physics {
		Vector3 pos,	// Posici�n
			vel,		// Direcci�n/velocidad (vector)
			acc;		// Aceleraci�n (vector) en m/s^2
		float damp,		// Rozamiento (magnitud)
			mass,		// Masa (magnitud) en kg
			simSpd;
	};


protected:
	physx::PxTransform* tr_;		// Transform de la esfera (Su posici�n se va actualizando)
	RenderItem* renderItem_;		// Objeto renderizable

	visual vis_;
	physics phys_;

	// F�sicas que se actualizan luego (porque se simulan o integran)
	Vector3 vel, acc;					
	float mass;


	float maxLifetime_;
	float lifetime_;
	bool alive_;


	void init(visual vis, physics phys, float maxLifetime = 1.0f);
	void simulatePhys();

public:
	Particle() { };
	Particle(visual vis, physics phys, float maxLifetime = 1.0f);
	virtual ~Particle();
	virtual void onDeath() { };

	void update(double t);


	// Obtener o cambiar posici�n (inicial o actual)
	inline void setInitPos(Vector3 pos) { phys_.pos = pos; }
	inline Vector3 getInitPos() { return phys_.pos; }

	inline void setPos(Vector3 pos) { tr_->p = pos; }
	inline Vector3 getPos() { return tr_->p; }


	// Obtener o cambiar velociad (inicial o actual/simulada)
	inline void setInitVel(Vector3 v) { phys_.vel = v; }
	inline Vector3 getInitVel() { return phys_.vel; }

	inline void setVel(Vector3 v) {
		vel = v; 
		simulatePhys();
	}
	inline Vector3 getVel() { return vel; }


	// Obtener o cambiar velocidad (inicial o actual/simulada)
	void setInitAcc(Vector3 a) { phys_.acc = a; }
	inline Vector3 getInitAcc() { return phys_.acc; }
	
	void setAcc(Vector3 a) { 
		acc = a; 
		simulatePhys();
	}
	inline Vector3 getAcc() { return acc; }



	inline bool isAlive() { return alive_; }
	inline void changeLifetime(float t) { maxLifetime_ = t; }
	
	virtual Particle* clone();
};
