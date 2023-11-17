#pragma once

#include "ForceGenerator.h"
#include <unordered_map>
#include <unordered_set>

class ParticleForceRegistry {
private:
	std::unordered_map<ForceGenerator*, std::unordered_set<Particle*>> forces_;			// Cada fuerza guarada las part�culas a las que afecta
	std::unordered_map<Particle*, std::unordered_set<ForceGenerator*>> particles_;		// Cada part�cula guarza las fuerzas que la afectan

public:
	// El sistema de part�culas se encarga de borrar las part�culas y las fuerzas
	virtual ~ParticleForceRegistry() { }

	inline void updateForces(double t) {
		// Para cada fuerza, recorre cada part�cula a la que afecta para actualizarla
		for (auto f : forces_) {
			for(auto p : f.second)
				f.first->update(p, t);
		}
	}

	// A�ade una fuerza sin part�culas asignadas
	inline void addForce(ForceGenerator* fg) { forces_.insert({ fg, { } }); }

	// A�ade una fuerza a una part�cula o una part�cula a una fuerza 
	inline void addForce(ForceGenerator* fg, Particle* p) {
		// Se a�ade la fuerza con un set vac�o. Si ya exist�a la fuerza, no
		// insertar� nada, y si no exist�a, f devolver� un <it, bool> al par
		// <fuerza, set de part�culas> creado y se inserta la part�cula en el set
		auto f = forces_.insert({ fg, { } });
		(*f.first).second.insert(p);

		// Lo mismo para las part�culas
		auto part = particles_.insert({ p, { } });
		(*part.first).second.insert(fg);
	}
	

	// Borra todas las fuerzas de una part�cula
	inline void deleteParticleRegistry(Particle* p) {
		std::vector<ForceGenerator*> partInForce;

		// Busca la part�cula en el mapa de part�culas. 
		// Si la encuentra, borra todas sus fuerzas y
		// va guardando cada fuerza que la afectaba
		auto part = particles_.find(p);
		if (part != particles_.end()) {
			for (auto it = (*part).second.begin(); it != (*part).second.end(); ) {
				partInForce.push_back((*it));
				it = (*part).second.erase(it);

			}
		}

		// Por cada generador de part�culas guardado, lo busca
		// en el mapa de fuerzas y borra la part�cula de ellos
		for (auto f : partInForce) {
			auto it = forces_.find(f);
			if (it != forces_.end()) (*it).second.erase(p);
		}

		particles_.erase(p);
	}

};