#pragma once

#include "../ForceGenerator.h"
#include "../../checkMemLeaks.h"

class BuoyancyForceGenerator : public ForceGenerator {
protected:
	float height_, volume_, density_, g_;
	Particle* liquid_;

public:
	BuoyancyForceGenerator(float h, float V, float d, Particle* liquid, float g = 1)
		: ForceGenerator(), height_(h), volume_(V), density_(d), g_(g), liquid_(liquid) 
	{ };
	
	virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			float h = p->getPos().y;			// Posición del centro del objeto
			float h0 = liquid_->getPos().y;		// Posición de la superficie del líquido

			float immersed = 0.0f;

			// El objeto está 0% inmerso si su parte inferior está por encima de la superficie
			// (su centro está altura/2 por encima de la superficie), 100% inmerso si su parte
			// superior está por debajo de la superficie (su centro está altura/2 por debajo 
			// de la superficie), y (h0 - h) / height_ + 0.5% sumergido en cualquier otro caso
			if (h - h0 > height_ * 0.5) immersed = 0.0f;
			else if (h - h0 < -height_ * 0.5)  immersed = 1.0f;
			else immersed = (h0 - h) / height_ + 0.5;

			float y = density_ * volume_ * immersed * g_;
			Vector3 f = { 0, y, 0 };
			p->addForce(f);
		}
	};

	inline virtual void keyPress(unsigned char key) {
		switch (tolower(key)) {
		case 'm':
			volume_++;
			break;
		case 'n':
			if(volume_ >= 1) volume_--;
			break;
		
		default: break;
		}
	}

};