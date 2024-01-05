#pragma once

#include "../Practicas/Particle.h"

class Debris : public Particle {
public:
	Debris(Vector4 color, float maxLifetime) : Particle() {
		Particle::visual v;
		v.size = { 0.5f, 0.5f, 0.5f };
		v.type = Particle::geomType::geomBox;
		v.color = color;

		Particle::physics p;
		p.mass = 1.0f / 2;

		init(v, p, maxLifetime);
	}
};