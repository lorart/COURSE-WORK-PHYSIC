#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/AiPeople.h"
#include "CollisionVolume.h"
using namespace NCL;
using namespace NCL::CSC8503;
class apple: public GameObject
{
public:
	apple();
	~apple();
	int GetAppleLabel() { return label; };
	void SetAplleLabel(int n) { label = n; };
	bool CheckGooseColli(Vector3 position);
	void SetAppleGamePosition(Vector3 GoosePosition,Vector3 HomePosition);
	
private:
	int label=0;

};

