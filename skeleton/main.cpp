#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;



#define P5
#ifndef P5
	#ifdef P1 
		#include "Practicas/Scenes/SceneP1.h"
		SceneP1* scMngr = nullptr;
	#elif defined(P2)
		#include "Practicas/Scenes/SceneP2.h"
		SceneP2* scMngr = nullptr;
	#elif defined(P3)
		#include "Practicas/Scenes/SceneP3.h"
		SceneP3* scMngr = nullptr;
	#elif defined(P4)
		#include "Practicas/Scenes/SceneP4.h"
		SceneP4* scMngr = nullptr;
	#endif
#else
	
#endif


void setupPr() {
#ifdef P1	
	scMngr = new SceneP1();
#elif defined(P2)
		scMngr = new SceneP2();
#elif defined(P3)
		scMngr = new SceneP3();
#elif defined(P4)
		scMngr = new SceneP4();
#else

	PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0, 0, 0 }));
	
	PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	suelo->attachShape(*shape);
	
	gScene->addActor(*suelo);
	RenderItem* item = new RenderItem(shape, suelo, {0.8, 0.8, 0.8, 0.1});
	

	PxRigidDynamic* solid;
	solid = gPhysics->createRigidDynamic(PxTransform({ -70, 50, 70 }));
	solid->setLinearVelocity({ 0,5,0 });
	solid->setAngularVelocity({ 0,0,0 });
	shape = CreateShape(PxBoxGeometry(5, 5, 5));
	solid->attachShape(*shape);

	PxRigidBodyExt::updateMassAndInertia(*solid, 0.15);
	gScene->addActor(*solid);
	item = new RenderItem(shape, solid, { 1.0, 0.0, 0.0, 1.0 });
#endif
}


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);


	setupPr();
}



// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);


#ifndef P5
	scMngr->update(t);
#endif
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();



#ifndef P5
	delete scMngr;
#else
	
#endif
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
	case ' ':
	{
		break;
	}
	default:
		break;
	}

#ifndef P5
	scMngr->keyPress(key);
#endif
}


void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

	//std::cout << "col\n";
}


int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}