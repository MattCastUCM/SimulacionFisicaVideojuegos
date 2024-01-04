#pragma once

#include "../ForceGenerator.h"
#include "../../checkMemLeaks.h"

class DragForceGenerator : public ForceGenerator {
protected:
	float k1_, k2_;

public:
	DragForceGenerator(const float& k1, const float& k2 = 0) : ForceGenerator(), k1_(k1), k2_(k2) { };
	

	inline virtual void update(Particle* p, double t) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			Vector3 v = p->getVel();

			float dragCoef = v.normalize();
			dragCoef = k1_ * dragCoef + k2_ * dragCoef * dragCoef;

			Vector3 f = -v * dragCoef;
			p->addForce(f);
		}
	};


	inline virtual void keyPress(unsigned char key) {
		switch (tolower(key)) {

		default: break;
		}
	}

};