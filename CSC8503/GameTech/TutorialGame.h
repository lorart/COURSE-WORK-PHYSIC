#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/NavigationMap.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../CSC8503Common/AiPeople.h"
#include "../CSC8503Common/goose.h"
#include "../CSC8503Common/apple.h"


namespace NCL {
	namespace CSC8503 {
		class TutorialGame	: public NavigationGrid {
		public:
			TutorialGame();
			~TutorialGame();
			vector<Vector3> testNodes;
			virtual void UpdateGame(float dt);
			void DisplayPathfinding(vector<Vector3>& testNodes, float time);
			void OnlyDisplayPathfinding(vector<Vector3> testNodes);
			Vector3 GoosePlayer;
			void UpdatePathFinding(float time);
			void UpdateColApple(float dt,  goose* gooseA);

			vector<Vector3> roadPosition;
			vector<Vector3> applePosition;
			void UpdateScore(goose* gooseP);





		protected:
			void InitialiseAssets();
			void CreateNavigationGrid();

			

			

			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			/*
			These are some of the world/object creation functions I created when testing the functionality
			in the module. Feel free to mess around with them to see different objects being created in different
			test scenarios (constraints, collision types, and so on). 
			*/
			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void BridgeConstraintTest();
			void SimpleGJKTest();

			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();
			void LockedCameraMovement();

			Vector3 GetGoosePosition();
			Vector3 GetCharacterPosition(AiPeople*);


			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);

			GameObject* AddHouseToWorld(const Vector3& position, Vector3 dimensions, float inverseMass);
			
			goose* AddGooseToWorld(const Vector3& position);
			GameObject* AddParkKeeperToWorld(const Vector3& position);
			AiPeople* AddCharacterToWorld(const Vector3& position);
			apple* AddAppleToWorld(const vector<Vector3>& position);


			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			///////
			goose* gooseA = nullptr;
			AiPeople* characterA = nullptr;
			int applenumber =8;
			std::vector<apple> appleA{8};
			Vector3 HomePosition;
			


			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLShader*	basicShader = nullptr;

			//Coursework Meshes
			OGLMesh*	gooseMesh	= nullptr;
			OGLMesh*	keeperMesh	= nullptr;
			OGLMesh*	appleMesh	= nullptr;
			OGLMesh*	charA		= nullptr;
			OGLMesh*	charB		= nullptr;


			NavigationGrid* grid = nullptr;


			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
		};
	}
}


