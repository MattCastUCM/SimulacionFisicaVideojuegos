#pragma once

#include "../ParticleForceSystem.h"

class SpringSystem : public ParticleForceSystem {
private:
	ForceGenerator* gr_, *wind_;
	Particle* staticPart_;
	bool activatedGr_, activatedWind_;

public:
	SpringSystem();

	void keyPress(unsigned char key);

private:
	void activatePartToStatic();
	void activatePartToPart();
	void activateRubberBand();
	void activateSlinky();
	void activateBuoyancy();

	void toggleGravity(bool activate);
	void toggleWind(bool activate);
};

