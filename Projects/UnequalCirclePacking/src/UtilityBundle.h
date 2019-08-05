#ifndef UTILITY_BUNDLE_H
#define UTILITY_BUNDLE_H

#include <vector>
#include "Circle.h"

enum UtilityType
{
	WATER,
	ELECTRICITY,
	GAS,
	SEWER,
	GENERIC,
	FIBER,
	PHONE
};

class Utility : public Circle
{
public:

	Utility(const float x, const float y, const float radius, const UtilityType utility_type);
	~Utility();

	void draw();

private:

	ShaderColor utility_color;

	void setup_utility_color(const UtilityType utility_type);

};

class UtilityBundle
{

public:

	std::vector<Utility> utilities;

	Circle containing_circle;

	void set_containing_circle_radius(float radius)
	{
		this->containing_circle.set_radius(radius);
	}

	UtilityBundle(std::vector<Utility> utilities);
	~UtilityBundle();

	void draw();

private:

};

#endif