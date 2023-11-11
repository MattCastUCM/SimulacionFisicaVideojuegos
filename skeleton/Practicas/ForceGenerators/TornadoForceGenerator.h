#pragma once

#include "WindForceGenerator.h"

class TornadoForceGenerator : public WindForceGenerator {
protected:
	Vector3 origin_;

public:
	TornadoForceGenerator(Vector3 origin, Vector3 windVel, const float& k1, const float& k2 = 0, Vector3 minPos = { -1000, -1000, -1000 }, Vector3 maxPos = { 1000, 1000, 1000 })
		: WindForceGenerator(windVel, k1, k2, minPos, maxPos), origin_(origin) { };
	
	virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			auto pos = p->getPos();
			if (pos.x >= minPos_.x && pos.x <= maxPos_.x
				&& pos.y >= minPos_.y && pos.y <= maxPos_.y
				&& pos.z >= minPos_.z && pos.z <= maxPos_.z)
			{
				Vector3 f;
				f.x = -(pos.z - origin_.z);
				f.y = 50 - pos.y - origin_.y;
				f.z = pos.x - origin_.x;

				f *= k1_;
				p->addForce(f);
			}
		}
	};
};