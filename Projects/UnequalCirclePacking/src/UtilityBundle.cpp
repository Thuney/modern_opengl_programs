#include "UtilityBundle.h"

Utility::Utility(const float x, const float y, const float radius, const UtilityType utility_type) : Circle(x, y, radius)
{
	setup_utility_color(utility_type);
}

Utility::~Utility()
{
}

void Utility::setup_utility_color(const UtilityType utility_type)
{
	switch (utility_type)
	{
	case WATER:
		this->utility_color = BLUE;
		break;
	case ELECTRICITY:
		this->utility_color = RED;
		break;
	case GAS:
		this->utility_color = YELLOW;
		break;
	case SEWER:
		this->utility_color = GREEN;
		break;
	case GENERIC:
		this->utility_color = CYAN;
		break;
	case FIBER:
		this->utility_color = ORANGE;
		break;
	case PHONE:
		this->utility_color = ORANGE;
		break;
	default:
		this->utility_color = WHITE;
		break;
	}
}

void Utility::draw()
{
	Circle::draw(this->utility_color);
}

UtilityBundle::UtilityBundle(std::vector<Utility> utilities)
{
	this->utilities = utilities;
	this->containing_circle = NULL;
}

UtilityBundle::~UtilityBundle()
{
	//if (this->utilities)
	//{
	//	for (int i = this->num_utilities - 1; i >= 0; i--)
	//	{
	//		delete (this->utilities + i);
	//	}
	//	delete this->utilities;
	//}
}

void UtilityBundle::draw()
{
	this->containing_circle.draw(GRAY);
	for (std::vector<Utility>::iterator i = this->utilities.begin(); i != this->utilities.end(); i++)
		(*i).draw();
}
