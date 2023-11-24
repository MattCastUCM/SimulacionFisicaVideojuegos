#pragma once

#include "../ParticleForceSystem.h"
#include "../ForceGenerators/SpringForceGenerator.h"
#include "../ForceGenerators/GravityForceGenerator.h"

class SpringSystem : public ParticleForceSystem {
private:
	ForceGenerator* gr_;

public:
	SpringSystem();

	void keyPress(unsigned char key);

private:
	void activatePartToStatic();
	void activatePartToPart();
	void activateSlinky();
	void activateGravity();
	void activateFlotation();
};

