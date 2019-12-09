#include "AiPeople.h"




AiPeople::AiPeople()
{
	this->name = "Aipeople";
	 AiPeoplePos = Vector3(80, 0, 80);
	 GoosePos = Vector3(0, 0, 0);
}

AiPeople::~AiPeople()
{
}

vector<Vector3> AiPeople::GetPathNode(NavigationGrid* grid,Vector3 GoosePos, Vector3 AiPeoplePos)
{
	testNodes.clear();
	found = grid->FindPath(GoosePos, AiPeoplePos, findPath);
	Vector3 pos;
	while (findPath.PopWaypoint(pos)) {
		testNodes.push_back(pos);
	}
	return testNodes;
}
