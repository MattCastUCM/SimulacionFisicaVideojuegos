#pragma once

#include "ForceGenerator.h"
#include <unordered_map>
#include <stack>

class ParticleForceRegistry {
private:
	std::unordered_multimap<ForceGenerator*, Particle*> forces;			// Las fuerzas pueden estar repetidsa (para obtener todas las partículas que tienen una fuerza)
	std::unordered_multimap<Particle*, ForceGenerator*> particles;		// Las partículas pueden estar repetidas (para obtener todas las fuerzas de una partícula)

public:
	~ParticleForceRegistry() {
		for (auto f : forces) delete f.first;
	}

	inline void updateForces(double t) {
		for (auto f : forces) {
			f.first->update(f.second, t);
		}
	}

	inline void addRegistry(ForceGenerator* fg, Particle* p) { forces.insert({ fg, p }); }
	inline void addParticle(ForceGenerator* fg, Particle* p) { particles.insert({ p, fg }); }

	inline void deleteParticleRegistry(Particle* p) {
		std::stack<ForceGenerator*> f;
		
		// Encuentra todos los generadores asignados a una partícula
		for (auto it = particles.begin(); it != particles.end();) {
			it = particles.find(p);
			if (it != particles.end()) {
				f.push(it->second);
				it = particles.erase(it);
			}
		}
		//particles.erase(p);


		while(!f.empty()) {
			auto force = f.top();
			auto it = forces.find(force);

			if (it != forces.end()) {
				if (it->second == p) {
					forces.erase(it);
					f.pop();
				}
			}
		}
	}
};