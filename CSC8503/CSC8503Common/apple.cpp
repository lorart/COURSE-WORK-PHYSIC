#include "apple.h"

apple::apple()
{
	this->name = "apple";
	label = 0;

}

apple::~apple()
{
}

bool apple::CheckGooseColli(Vector3 position)
{
	if ((this->GetTransform().GetWorldPosition()-position).Length()<=2) {

		this->label = 1;//been collision
		//std::cout <<  "apple has been collision "  << std::endl;
		return 1;
	}
	return 0;
}

void apple::SetAppleGamePosition(Vector3 GoosePosition, Vector3 HomePosition)
{
	static float appleHeigh = 2;
	
	if (this->label == 1) {
		this->GetTransform().SetWorldPosition(GoosePosition+Vector3(0, appleHeigh,0));
		
		//appleHeigh += 0.5;
		if ((this->GetTransform().GetWorldPosition() - HomePosition).Length() <2) {
			this->label = 2;
			this->GetTransform().SetWorldPosition(HomePosition);
		//	std::cout << "apple has been home " << std::endl;
		}
	}
}





