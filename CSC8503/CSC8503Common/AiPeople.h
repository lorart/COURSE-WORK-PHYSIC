#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/NavigationMap.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "CollisionVolume.h"

using namespace NCL;
using namespace NCL::CSC8503;
#define gridtxt "TestGrid2.txt"
class AiPeople : public GameObject
{public:
	AiPeople();
	~AiPeople();
//	grid = new NavigationGrid(gridtxt);
	NavigationPath findPath;
	NavigationGrid* grid = nullptr;
	bool found;



	vector<Vector3> testNodes;
	Vector3 AiPeoplePos;
	Vector3 GoosePos;

	vector<Vector3> GetPathNode(NavigationGrid* grid,Vector3 GoosePos, Vector3 AiPeoplePos);

};

