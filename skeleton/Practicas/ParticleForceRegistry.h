#pragma once

#include <map>
#include "ForceGenerator.h"

typedef std::pair<ForceGenerator*, Particle*> ForcePart;

class ParticleForceRegistry : public std::multimap<ForceGenerator*, Particle*> {
private:


public:
	inline void updateForces(double duration) {
		for (auto it = begin(); it != end(); ++it) {
			it->first->update(it->second, duration);
		}
	}

	inline void addRegistry(ForceGenerator* fg, Particle* p) { insert({ fg, p }); }
	inline void deleteParticleRegistry(Particle* p) { }
};