#pragma once

#include "../Practicas/ParticleForceRegistry.h"
#include "../Practicas/Particles/Firework.h"

class FireworkNoAcc : public Firework {
public:
	FireworkNoAcc(ParticleForceRegistry* fr, ForceGenerator* fg, 
		const Vector3& g, unsigned type, int nGens = 0, double lifeTime = 1.0f, float size = 1.0f) : Firework(g, type, nGens, lifeTime, size) 
	{
		fr->addForce(fg, this);
	}
};