#pragma once

#include <list>
#include "Particle.h"
#include <random>

class ParticleGenerator {
protected:
	//std::string name_;		// Nombre del generador
	int generateN_;				// Partículas a generar en cada llamada a generateParticles
	double genProb_;			// Probabilidad de que genere una partícula (1 => siempre genera una)
	Particle* modelPart_;		// Guarda los atributos de la partícula a generar (damping, tiempo de vida, etc.)
	Vector3 origin_, vel_;		// Origen y velocidad de las partículas que se crean

	std::mt19937 mt_;
	double time_, generationTime_;			// Tiempo que ha pasado desde la última generación, tiempo que tarda en generar nuevas partículas
	bool changeX_, changeY_, changeZ_;		// Si el generador cambia la velocidad x, y o z de la partícula al generar partículas nuevas
	bool active_;

public:
	ParticleGenerator(/*const std::string& name,*/ double genTime, bool changeX = true, bool changeY = false, bool changeZ = true);


	virtual std::list<Particle*> generateParticles();

	virtual void setVelocities(Particle* p) { }
	
	std::list<Particle*> update(double t);

	void changeModelPart(Particle* p, bool modifyPosVel = true);
	inline Particle* getModelPart() { return modelPart_; };

	inline void setOrigin(const Vector3& p) {
		modelPart_->setInitPos(p);
		origin_ = p;
	}
	inline void setVel(const Vector3& v) { 
		modelPart_->setVel(v);
		vel_ = v; 
	}
	inline void setAcc(const Vector3& acc) {
		modelPart_->setInitAcc(acc);
	}
	inline Vector3 getVel() const { return vel_; }
	inline void changeGenerateN(int n) { generateN_ = n; }
	inline void changeLifetime(double t) { modelPart_->changeLifetime(t); }
	inline void setActive(bool a) { active_ = a; }
	
	
	
};