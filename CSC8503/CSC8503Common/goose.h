#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/apple.h"
#include "CollisionVolume.h"
using namespace NCL;
using namespace NCL::CSC8503;
class goose: public GameObject
{
public:
	goose();
	~goose();
	int GetScore() { return score; };
	void SetScore(int n) { score = n; };
	int  GetScore(Vector3 ApplePosition,Vector3 GoosePostion);
	void OnCollisionEnd(apple* applecol);
protected:
	int score=0;
};

