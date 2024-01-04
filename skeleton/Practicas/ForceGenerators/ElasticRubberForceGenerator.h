#pragma once

#include "SpringForceGenerator.h"
#include "../../checkMemLeaks.h"

class ElasticRubberForceGenerator : public SpringForceGenerator {

public:
	ElasticRubberForceGenerator(double k, double restingLength, Particle* other) : SpringForceGenerator(k, restingLength, other) {}

	virtual void update(Particle* p, double t) {
		Vector3 posDif = other_->getPos() - p->getPos();

		// Solo se aplica la fuerza cuando la distancia entre 
		// partículas supera la distancia de reposo (cuando se 
		// estira) pero no cuando es menor (cuando se encoje)
		if (posDif.magnitude() > restingLength_)
			SpringForceGenerator::update(p, t);
	};

};