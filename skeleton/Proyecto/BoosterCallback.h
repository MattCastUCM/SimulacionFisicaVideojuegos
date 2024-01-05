#pragma once

#include <functional>
#include "../callbacks.hpp"
#include "../checkMemLeaks.h"


extern void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

class BoosterCallback : public physx::PxSimulationEventCallback {
protected:
	std::function<void()> callback_;
	physx::PxActor* ballActor_;

public:
	BoosterCallback(std::function<void()> funct, physx::PxActor* ballActor) : PxSimulationEventCallback() {
		callback_ = funct;
		ballActor_ = ballActor;
	};

	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
		PX_UNUSED(pairs); PX_UNUSED(count);
		if(pairs->otherActor == ballActor_) callback_();
	}

	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(physx::PxActor** actors, physx::PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(physx::PxActor** actors, physx::PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) { }
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
		PX_UNUSED(pairs);
		PX_UNUSED(nbPairs);
		physx::PxActor* actor1 = pairHeader.actors[0];
		physx::PxActor* actor2 = pairHeader.actors[1];
		onCollision(actor1, actor2);
		
	}
	
};
