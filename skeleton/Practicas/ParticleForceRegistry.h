#pragma once

#include "ForceGenerator.h"
#include <unordered_map>
#include <unordered_set>

class ParticleForceRegistry {
private:
	std::unordered_map<ForceGenerator*, std::unordered_set<Particle*>> forces_;			// Cada fuerza guarada las partículas a las que afecta
	std::unordered_map<Particle*, std::unordered_set<ForceGenerator*>> particles_;		// Cada partícula guarza las fuerzas que la afectan

public:
	// El sistema de partículas se encarga de borrar las partículas y las fuerzas
	virtual ~ParticleForceRegistry() { }

	inline void updateForces(double t) {
		std::vector<ForceGenerator*> removeForces;
		// Para cada fuerza, recorre cada partícula a la que afecta para actualizarla
		for (auto f : forces_) {
			f.first->updateTime(t);
			for (auto p : f.second) {
				if (f.first->isActive())
					f.first->update(p, t);
			}
		}
	}

	// Añade una fuerza sin partículas asignadas
	inline void addForce(ForceGenerator* fg) { forces_.insert({ fg, { } }); }

	// Añade una fuerza a una partícula o una partícula a una fuerza 
	inline void addForce(ForceGenerator* fg, Particle* p) {
		// Se añade la fuerza con un set vacío. Si ya existía la fuerza, no
		// insertará nada, y si no existía, f devolverá un <it, bool> al par
		// <fuerza, set de partículas> creado y se inserta la partícula en el set
		auto f = forces_.insert({ fg, { } });
		(*f.first).second.insert(p);

		// Lo mismo para las partículas
		auto part = particles_.insert({ p, { } });
		(*part.first).second.insert(fg);
	}
	

	// Borra todas las fuerzas de una partícula
	inline void deleteParticleRegistry(Particle* p) {
		std::vector<ForceGenerator*> partInForce;

		// Busca la partícula en el mapa de partículas. 
		// Si la encuentra, borra todas sus fuerzas y
		// va guardando cada fuerza que la afectaba
		auto part = particles_.find(p);
		if (part != particles_.end()) {
			for (auto it = (*part).second.begin(); it != (*part).second.end(); ) {
				partInForce.push_back((*it));
				it = (*part).second.erase(it);

			}
		}

		// Por cada generador de partículas guardado, lo busca
		// en el mapa de fuerzas y borra la partícula de ellos
		for (auto f : partInForce) {
			auto it = forces_.find(f);
			if (it != forces_.end()) (*it).second.erase(p);
		}

		particles_.erase(p);
	}


	inline void deleteForceRegistry(ForceGenerator* f) {
		std::vector<Particle*> forceInPart;

		auto force = forces_.find(f);
		if (force != forces_.end()) {
			for (auto it = (*force).second.begin(); it != (*force).second.end(); ) {
				forceInPart.push_back((*it));
				it = (*force).second.erase(it);
			}
		}

		for (auto p : forceInPart) {
			auto it = particles_.find(p);
			if (it != particles_.end()) (*it).second.erase(f);
		}

		forces_.erase(f);
	}


	// Elimina una fuerza de una partícula o una partícula de una fuerza 
	inline void removeForce(ForceGenerator* fg, Particle* p) {
		auto &f = forces_.find(fg);
		if (f != forces_.end()) {
			auto &parts = forces_.at(fg);
			parts.erase(p);
		}

		auto &part = particles_.find(p);
		if (part != particles_.end()) {
			auto &f = particles_.at(p);
			f.erase(fg);
		}
	}
};