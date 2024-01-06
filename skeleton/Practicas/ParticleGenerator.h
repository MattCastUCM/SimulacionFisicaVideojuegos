#pragma once

#include <list>
#include "Particle.h"
#include "Particles/DRigidBody.h"
#include <random>
#include "../checkMemLeaks.h"

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
	bool active_, autoInactive_;
	std::string name_;


	// Convertir a radianes
	inline float rads(float deg) { return 0.017453292 * deg; }
	inline double rads(double deg) { return 0.017453292 * deg; }



public:
	ParticleGenerator(double genTime, bool autoInactive = false, bool changeX = true, bool changeY = false, bool changeZ = true) 
		: generateN_(3), genProb_(1.0), modelPart_(nullptr), time_(0), generationTime_(genTime), 
		  changeX_(changeX), changeY_(changeY), changeZ_(changeZ), autoInactive_(autoInactive), active_(true), name_("") { };

	~ParticleGenerator() {
		if(modelPart_ != nullptr) delete modelPart_;
	}

	inline virtual std::list<Particle*> generateParticles() {
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			generated.push_back(p);
		}

		return generated;
	};
	virtual void setVelocities(Particle* p) { }

	inline std::list<Particle*> update(double t) {
		if (active_) {
			time_ += t;
			if (time_ >= generationTime_) {

				std::list<Particle*> generated = generateParticles();
				time_ = 0;
				if (autoInactive_) active_ = false;
				return generated;
			}
			else return { };
		}
		else return { };
	};

	inline void changeModelPart(Particle* p, bool modifyPosVel = true, bool isRigid = false) {
		delete modelPart_;
		modelPart_ = p->clone();
		if (modifyPosVel) {
			origin_ = p->getInitPos();
			vel_ = p->getInitVel();
		}
		if(!isRigid) 
			modelPart_->setPos({ -1000.0f, -1000.0f, -1000.0f });
		else
			((DRigidBody*)modelPart_)->setPos({ -1000.0f, -1000.0f, -1000.0f });
	};

	// Getters y setters
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
	inline void changeLifeTime(double t) { modelPart_->changeLifeTime(t); }
	inline void setActive(bool a) { 
		active_ = a; 
		time_ = generationTime_;
	}
	
	inline int getN() { return generateN_; }
	inline bool isActive() { return active_; }

	inline void setName(std::string n) { name_ = n; }
	inline std::string getName() { return name_; }

	inline virtual void keyPress(unsigned char key) { }
};