#pragma once

#include "../ForceGenerator.h"
#include <iostream>
using namespace std;

class SpringForceGenerator : public ForceGenerator {
protected:
	double k_, restingLength_;
	Particle* other_;

public:
	SpringForceGenerator(double k, double restingLength, Particle* other, bool autoInactive = false) : ForceGenerator("", -1, autoInactive), k_(k), restingLength_(restingLength), other_(other) { };


	virtual void update(Particle* p, double t) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			Vector3 posDif = other_->getPos() - p->getPos();
			//std::cout << posDif.y << '\n';

			//para que no se pare justo cuando están en la misma posición
			//delta -= p->getSize() / 2;
			float delta = posDif.normalize() - restingLength_;
			/*float d = posDif.normalize();
			cout << restingLength_ << ' ' << d << '\n';*/

			Vector3 f = posDif * delta * k_;
			p->addForce(f);
		}
		
	};

	inline virtual void keyPress(unsigned char key) {
		switch (tolower(key)) {
		case 'p':
			k_ += 5;
			break;
		case 'o':
			if(k_ - 5 > 0) k_ -= 5;
			break;

		default: break;
		}
	}

};