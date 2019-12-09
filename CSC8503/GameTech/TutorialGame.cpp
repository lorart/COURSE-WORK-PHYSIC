#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"

#include "../CSC8503Common/PositionConstraint.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame()	{
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);

	forceMagnitude	= 10.0f;
	useGravity		= TRUE;
	inSelectionMode = false;

	Debug::SetRenderer(renderer);
	
//	std::vector<apple> appleA{ 8 };
	grid = new NavigationGrid("TestGrid2.txt");

	gooseA = new goose();
	characterA = new AiPeople();
	
	HomePosition = Vector3(6,0,6);

	InitialiseAssets();
	

}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void TutorialGame::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh"		 , &cubeMesh);
	loadFunc("sphere.msh"	 , &sphereMesh);
	loadFunc("goose.msh"	 , &gooseMesh);
	loadFunc("CharacterA.msh", &keeperMesh);
	loadFunc("CharacterM.msh", &charA);
	loadFunc("CharacterF.msh", &charB);
	loadFunc("Apple.msh"	 , &appleMesh);

	basicTex	= (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	InitCamera();
	InitWorld();
}

#pragma region navigation

void NCL::CSC8503::TutorialGame::CreateNavigationGrid()
{
	Vector3 cubeDims = Vector3(1, 1, 1);
	GridNode n;

	Vector3 position;
	for (int y = 0; y < grid->gridWidth; ++y) {
		for (int x = 0; x < grid->gridHeight; ++x) {
			//position =Vector3((grid->gridWidth/ tempnodesize)*x,0, (grid->gridHeight / tempnodesize)*y);
			//auto zzz = grid->getGridNode();
			n = grid->getGridNode()[(grid->gridWidth * y) + x];
			if (n.type == 'x')
			{
				AddCubeToWorld(n.position, cubeDims * 3, 0);
			}
			else if (n.type =='.') {				
			   roadPosition.push_back(n.position);
			
			}
		
		}
	}
	int temp = roadPosition.size();
	for (int  i = 0; i < applenumber; i++)
	{
		applePosition.push_back(roadPosition[rand()/temp]+Vector3(0,2,0));
		std::cout << "pushing into roadposition" << applePosition[i] << std::endl;
	}


	AddFloorToWorld(Vector3(50, -5, 50));

}


void NCL::CSC8503::TutorialGame::OnlyDisplayPathfinding(vector<Vector3>testNodes) {
	for (int i = 1; i < testNodes.size(); ++i) {
		Vector3 a = testNodes[i - 1];
		Vector3 b = testNodes[i];
		
		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}



void NCL::CSC8503::TutorialGame::DisplayPathfinding(vector<Vector3>& testNodes,float time) {
	static float totalTime = 0.0f;
	totalTime += time*0.2;
	int i = floor(totalTime);
	float smooth = totalTime - floor(totalTime);

	if (testNodes.size() > 1) {
		//std::cout << "testNodes.size" << testNodes.size() << std::endl;
		Vector3 a = testNodes[1];
		Vector3 b = testNodes[0];
		Vector3 c = b*(smooth) + a *(1-smooth);
		//Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
		characterA->GetTransform().SetWorldPosition(c);
	//	std::cout << (c - b).Length() << std::endl;
		if ((c - b).Length() < 0.01) {
			testNodes.erase(testNodes.begin());
		}
	}
	if (testNodes.size() == 1) {
		testNodes.clear();
	}

}

void NCL::CSC8503::TutorialGame::UpdatePathFinding(float faketime)
{
	static Vector3 previousGoosePosition = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 currentGooseNoY = GetGoosePosition();
	currentGooseNoY.y = 0;
	static vector<Vector3> testNodes;
	if ((previousGoosePosition - currentGooseNoY).Length() > 0.01) {
		//std::cout << previousGoosePosition << " " << currentGooseNoY << std::endl;
		previousGoosePosition = currentGooseNoY;
		testNodes = characterA->GetPathNode(grid, currentGooseNoY,GetCharacterPosition(characterA));
	

	}
	else {
		//std::cout << previousGoosePosition << " OK with " << currentGooseNoY << std::endl;
		//std::cout << "Undo" << std::endl;
	}
	OnlyDisplayPathfinding(testNodes);
	DisplayPathfinding(testNodes, faketime);
}

void NCL::CSC8503::TutorialGame::UpdateColApple(float dt,goose* gooseP)
{

	for (int i = 0; i < applenumber; i++)
	{
		appleA[i].CheckGooseColli(gooseP->GetTransform().GetWorldPosition());
		appleA[i].SetAppleGamePosition(gooseP->GetTransform().GetWorldPosition(), HomePosition);
	}



}


Vector3 NCL::CSC8503::TutorialGame::GetGoosePosition()
{
//	Vector3 tempposition= (gooseMesh->GetPositionData())[0];
	Vector3 tempposition = (gooseA->GetTransform()).GetWorldPosition();
//	std::cout << "goose position is=" << tempposition << std::endl;
	return (tempposition);
}

Vector3 NCL::CSC8503::TutorialGame::GetCharacterPosition(AiPeople* a)
{
	Vector3 tempposition = (a->GetTransform()).GetWorldPosition();
	return Vector3(tempposition);
}


#pragma endregion


//#pragma region COLLECT APPLE
//
//void NCL::CSC8503::TutorialGame::UpdateColApple(float dt)
//{
//	for (int i = 0; i < applenumber; i++)
//	{
//		gooseA->OnCollisionBegin(appleA[i]);
//	}
//	
//}
//
//
//#pragma endregion






TutorialGame::~TutorialGame()	{
	delete cubeMesh;
	delete sphereMesh;
	delete gooseMesh;
	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

void TutorialGame::UpdateGame(float dt) {
	if (!inSelectionMode) {
		world->GetMainCamera()->UpdateCamera(dt);
	}
	if (lockedObject != nullptr) {
		LockedCameraMovement();
	}

	UpdateKeys();

	if (useGravity) {
		Debug::Print("(G)ravity on", Vector2(10, 40));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(10, 40));
	}

	SelectObject();
	MoveSelectedObject();
	//UpdateColApple(dt);
	world->UpdateWorld(dt);
	renderer->Update(dt);
	physics->Update(dt);

	UpdateColApple(dt,gooseA);
	UpdateScore(gooseA);
	Debug::FlushRenderables();
	renderer->Render();
}

void TutorialGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}
}


void TutorialGame::LockedObjectMovement() {
	Matrix4 view		= world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		selectionObject->GetPhysicsObject()->AddForce(-rightAxis);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		selectionObject->GetPhysicsObject()->AddForce(rightAxis);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		selectionObject->GetPhysicsObject()->AddForce(fwdAxis);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		selectionObject->GetPhysicsObject()->AddForce(-fwdAxis);
	}
}

void  TutorialGame::LockedCameraMovement() {
	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetWorldPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);
	}
}



void TutorialGame::DebugObjectMovement() {
//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
	/*	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}*/
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(10, 0, 0));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

	/*	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}*/



		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}
}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.

*/
bool TutorialGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		renderer->DrawString("Press Q to change to camera mode!", Vector2(10, 0));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				///
				
				Debug::Print("it is a "+selectionObject->GetName(), Vector2(0, 100));
			




				return true;
			}
			else {
				return false;
			}
		}
		if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
			if (selectionObject) {
				if (lockedObject == selectionObject) {
					lockedObject = nullptr;
				}
				else {
					lockedObject = selectionObject;
				}
			}
		}
	}
	else {
		renderer->DrawString("Press Q to change to select mode!", Vector2(10, 0));
	}
	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/

void TutorialGame::MoveSelectedObject() {
	renderer->DrawString(" Click Force :" + std::to_string(forceMagnitude),
		Vector2(10, 20)); // Draw debug text at 10 ,20
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;// we haven ’t selected anything !
	}
	// Push the selected object !
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(
			*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
			/*	selectionObject->GetPhysicsObject() ->
					AddForce(ray.GetDirection() * forceMagnitude);*/

				selectionObject -> GetPhysicsObject() -> AddForceAtPosition(
					ray.GetDirection() * forceMagnitude,
					closestCollision.collidedAt);
			}
		}
	}

}

void TutorialGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.5f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();

	
	CreateNavigationGrid();
	AddGooseToWorld(Vector3(30, 10, 20));
	
	AddAppleToWorld(applePosition);
	
	AddParkKeeperToWorld(Vector3(40, 10, 20));
	AddCharacterToWorld(Vector3(45, 10, 20));
	AddHouseToWorld(HomePosition, Vector3(1, 1, 1),0);

}

//From here on it's functions to add in objects to the world!

#pragma region add things to world

void  TutorialGame::UpdateScore(goose* gooseP) {
	
		//Debug::Print(gooseP->GetName()+" score is " + gooseP->GetScore(), Vector2(10, 40));
	for (int i = 0; i < applenumber; i++)
	{		Debug::Print(" score is " + gooseP->GetScore(
		appleA[i].GetTransform().GetWorldPosition(), 
			gooseP->GetTransform().GetWorldPosition()), 
			Vector2(10, 300));
	}
		

}
/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* TutorialGame::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject();

	Vector3 floorSize = Vector3(100, 2, 100);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform().SetWorldScale(floorSize);
	floor->GetTransform().SetWorldPosition(position);


	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->SetName("floor");
	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple' 
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject();

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);
	sphere->GetTransform().SetWorldScale(sphereSize);
	sphere->GetTransform().SetWorldPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();
	sphere->SetName("sphere");
	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetName("cube");
	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddHouseToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetName("House");
	cube->GetRenderObject()->SetColour(Vector4(1,1, 0, 1));
	world->AddGameObject(cube);

	return cube;
}

//GameObject* TutorialGame::AddGooseToWorld(const Vector3& position)
//{
//	float size			= 1.0f;
//	float inverseMass	= 1.0f;
//
//
//
//	SphereVolume* volume = new SphereVolume(size);
//	goose->SetBoundingVolume((CollisionVolume*)volume);
//
//	goose->GetTransform().SetWorldScale(Vector3(size,size,size) );
//	goose->GetTransform().SetWorldPosition(position);
//
//	goose->SetRenderObject(new RenderObject(&goose->GetTransform(), gooseMesh, nullptr, basicShader));
//	goose->SetPhysicsObject(new PhysicsObject(&goose->GetTransform(), goose->GetBoundingVolume()));
//
//	goose->GetPhysicsObject()->SetInverseMass(inverseMass);
//	goose->GetPhysicsObject()->InitSphereInertia();
//
//	world->AddGameObject(goose);
//
//	return goose;
//}
goose* TutorialGame::AddGooseToWorld(const Vector3& position)
{
	float size = 1.0f;
	float inverseMass = 1.0f;


	
	SphereVolume* volume = new SphereVolume(size);
	gooseA->SetBoundingVolume((CollisionVolume*)volume);
	std::cout << gooseA->GetName() << std::endl;
	gooseA->GetTransform().SetWorldScale(Vector3(size, size, size));
	gooseA->GetTransform().SetWorldPosition(position);

	gooseA->SetRenderObject(new RenderObject(&gooseA->GetTransform(), gooseMesh, nullptr, basicShader));
	gooseA->SetPhysicsObject(new PhysicsObject(&gooseA->GetTransform(), gooseA->GetBoundingVolume()));

	gooseA->GetPhysicsObject()->SetInverseMass(inverseMass);
	gooseA->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(gooseA);

	return gooseA;
}

GameObject* TutorialGame::AddParkKeeperToWorld(const Vector3& position)
{
	float meshSize = 4.0f;
	float inverseMass = 0.5f;

	GameObject* keeper = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3, 0.9f, 0.3) * meshSize);
	keeper->SetBoundingVolume((CollisionVolume*)volume);

	keeper->GetTransform().SetWorldScale(Vector3(meshSize, meshSize, meshSize));
	keeper->GetTransform().SetWorldPosition(position);

	keeper->SetRenderObject(new RenderObject(&keeper->GetTransform(), keeperMesh, nullptr, basicShader));
	keeper->SetPhysicsObject(new PhysicsObject(&keeper->GetTransform(), keeper->GetBoundingVolume()));

	keeper->GetPhysicsObject()->SetInverseMass(inverseMass);
	keeper->GetPhysicsObject()->InitCubeInertia();

	keeper->GetTransform().SetWorldOrientation(
		Quaternion::AxisAngleToQuaterion(Vector3(0,1,0), 90.0f)

		//keeper->GetTransform().GetWorldOrientation() + Quaternion(0, 90, 0, 0)
	
	)  ;
		
   

	world->AddGameObject(keeper);

	return keeper;
}

AiPeople* TutorialGame::AddCharacterToWorld(const Vector3& position) {
	float meshSize = 4.0f;
	float inverseMass = 0.5f;

	auto pos = keeperMesh->GetPositionData();

	Vector3 minVal = pos[0];
	Vector3 maxVal = pos[0];

	for (auto& i : pos) {
		maxVal.y = max(maxVal.y, i.y);
		minVal.y = min(minVal.y, i.y);
	}

	//GameObject* character = new GameObject();

	float r = rand() / (float)RAND_MAX;


	AABBVolume* volume = new AABBVolume(Vector3(0.3, 0.9f, 0.3) * meshSize);
	characterA->SetBoundingVolume((CollisionVolume*)volume);

	characterA->GetTransform().SetWorldScale(Vector3(meshSize, meshSize, meshSize));
	characterA->GetTransform().SetWorldPosition(position);

	characterA->SetRenderObject(new RenderObject(&characterA->GetTransform(), r > 0.5f ? charA : charB, nullptr, basicShader));
	characterA->SetPhysicsObject(new PhysicsObject(&characterA->GetTransform(), characterA->GetBoundingVolume()));

	characterA->GetPhysicsObject()->SetInverseMass(inverseMass);
	characterA->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(characterA);

	return characterA;
}

apple* TutorialGame::AddAppleToWorld(const vector<Vector3>& position) {
	//for (int i = 0; i <sizeof(appleA)/ sizeof(appleA[0]); i++)
	for (int i = 0; i < applenumber; i++)
	{
		SphereVolume* volume = new SphereVolume(0.7f);
		//appleA[i].GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
		appleA[i].SetBoundingVolume((CollisionVolume*)volume);
		appleA[i].GetTransform().SetWorldScale(Vector3(4, 4, 4));
		appleA[i].GetTransform().SetWorldPosition(position[i]);

		appleA[i].SetRenderObject(new RenderObject(&appleA[i].GetTransform(), appleMesh, nullptr, basicShader));
		appleA[i].SetPhysicsObject(new PhysicsObject(&appleA[i].GetTransform(), appleA[i].GetBoundingVolume()));
		//std::cout << appleA[i]->GetName() << std::endl;
		appleA[i].GetPhysicsObject()->SetInverseMass(1.0f);
		appleA[i].GetPhysicsObject()->InitSphereInertia();
		appleA[i].GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
		world->AddGameObject(&appleA[i]);
		

	}
	std::cout << "add apple" << std::endl;
	return appleA.data();
	
}

//void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
//	for (int x = 0; x < numCols; ++x) {
//		for (int z = 0; z < numRows; ++z) {
//			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
//			AddSphereToWorld(position, radius, 1.0f);
//		}
//	}
//	AddFloorToWorld(Vector3(0, -2, 0));
//}
//
//#pragma endregion

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims);
			}
			else {
				AddSphereToWorld(position, sphereRadius);
			}
		}
	}
//	AddFloorToWorld(Vector3(0, -2, 0));
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols+1; ++x) {
		for (int z = 1; z < numRows+1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
//	AddFloorToWorld(Vector3(0, -2, 0));
}

void TutorialGame::BridgeConstraintTest() {
	Vector3 cubeSize = Vector3(8, 8, 8);

	float	invCubeMass = 5;
	int		numLinks	= 25;
	float	maxDistance	= 30;
	float	cubeDistance = 20;

	Vector3 startPos = Vector3(500, 1000, 500);

	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 0);

	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, 0);

	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}

	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

void TutorialGame::SimpleGJKTest() {
	Vector3 dimensions		= Vector3(5, 5, 5);
	Vector3 floorDimensions = Vector3(100, 2, 100);

	GameObject* fallingCube = AddCubeToWorld(Vector3(0, 20, 0), dimensions, 10.0f);
	GameObject* newFloor	= AddCubeToWorld(Vector3(0, 0, 0), floorDimensions, 0.0f);

	delete fallingCube->GetBoundingVolume();
	delete newFloor->GetBoundingVolume();

	fallingCube->SetBoundingVolume((CollisionVolume*)new OBBVolume(dimensions));
	newFloor->SetBoundingVolume((CollisionVolume*)new OBBVolume(floorDimensions));

}

