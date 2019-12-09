#include "goose.h"

goose::goose()
{
	this->name = "goose";
	this->score = 0;
}

goose::~goose()
{
}

int goose::GetScore(Vector3 ApplePosition, Vector3 GoosePostion)
{
	bool a = (this->GetTransform().GetWorldPosition() - ApplePosition).Length() <= 2;
	bool b = (this->GetTransform().GetWorldPosition() - GoosePostion).Length() <= 2;
	std::cout << "A is " << a << std::endl;
	std::cout << "b is " <<b << std::endl;
	if (a && b) {
		score++;
	}
	std::cout << "score is "<<score << std::endl;
	return score;
		
}

