#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Definitions.h"

#ifndef Explicaciones
std::string display_text = "This is a test";
#else
	#ifdef P5
		std::vector<std::string> display_text = {
			"Pulsar boton 1 para activar y desactivar la explosion",
			"(hay que desactivarla primero para volver a activarla)"
		};
	#elif defined(Proyecto)
		std::vector<std::string> display_text = { };
		std::string score = "score: " + std::to_string(0);
	#endif
#endif

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


// Buscar modificaciones de la plantilla (camara y texto de ejemplo) buscando "PRACTICAS"
#ifndef PracticasRigidos
	#define P4

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
	#ifdef P5
		#include "Practicas/Systems/RigidCubesSystem.h"
	#else
		#include "Proyecto/ProySystem.h"
#endif
	ParticleSystem* sys_;
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
	PxU32 gr1 = 1;
	PxU32 gr2 = 2;

	PxU32 m1 = 0; // Collides with no groups
	PxU32 m2 = 0; // Collides with no groups

	PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	shape->setSimulationFilterData(PxFilterData(gr1, m1, 0, 0));

	PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0, 0, 0 }));
	suelo->attachShape(*shape);

	gScene->addActor(*suelo);
	RenderItem* item = new RenderItem(shape, suelo, { 0.8, 0.8, 0.8, 0.1 });

	PxRigidDynamic* solid;

	shape = CreateShape(PxBoxGeometry(1, 1, 1));
	shape->setSimulationFilterData(PxFilterData(gr2, m2, 0, 0));

	solid = gPhysics->createRigidDynamic(PxTransform({ 0, 10, 0 }));
	solid->setLinearVelocity({ 0,0,0 });
	solid->setAngularVelocity({ 0,0,0 });
	solid->attachShape(*shape);

	PxRigidBodyExt::updateMassAndInertia(*solid, 1);
	gScene->addActor(*solid);
	item = new RenderItem(shape, solid, { 1.0, 0.0, 0.0, 1.0 });




	#ifdef P5
		sys_ = new RigidCubesSystem(gPhysics, gScene);
	#elif defined(Proyecto)
		sys_ = new ParticleSystem();

		//sys_ = new ProySys(gPhysics, gScene);


		
	#endif
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


#ifndef PracticasRigidos
	scMngr->update(t);
#else
	sys_->update(t);
#endif
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	// IMPORTANTE BORRARLO ANTES QUE LA ESCENA Y LAS FÍSICAS
#ifndef PracticasRigidos
	delete scMngr;
#else
	delete sys_;
#endif



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
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
	default:
		break;
	}


#ifndef PracticasRigidos
	scMngr->keyPress(key);
#else
	sys_->keyPress(key);
#endif
}


void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

#ifdef Proyecto
	//((ProySys*)sys_)->onCollision(actor1, actor2);
#endif
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