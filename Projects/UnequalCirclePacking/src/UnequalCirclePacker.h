#ifndef UNEQUAL_CIRCLE_PACKER_H
#define UNEQUAL_CIRCLE_PACKER_H

#include "UtilityBundle.h"

struct vec2
{
	float x;
	float y;

	vec2()
	{
		this->x = 0;
		this->y = 0;
	}

	vec2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float magnitude()
	{
		return sqrt(pow(y, 2) + pow(x, 2));
	}

	//vec2 operator+ (const vec2& v_other) const
	//{
	//	vec2 result;
	//	result.x += v_other.x;
	//	result.y += v_other.y;
	//	return result;
	//}

	//vec2 operator/ (const float& scalar) const
	//{
	//	vec2 result;
	//	result.x /= scalar;
	//	result.y /= scalar;
	//	return result;
	//}

	//vec2 operator* (const float& scalar) const
	//{
	//	vec2 result;
	//	result.x /= scalar;
	//	result.y /= scalar;
	//	return result;
	//}
};

class UnequalCirclePacker
{
public:

	bool done = false;
	bool packed = false;
	bool draw_intermediate;

	float containing_radius = 0.0f;

	double radius_stepdown_value = 0.0f;
	double done_time_cutoff = 0.0f;
	double unit_step_cutoff = 0.0f;
	double system_potential_cutoff = 0.0f;
	double unit_step_stepdown_value = 0.0f;

	UnequalCirclePacker(UtilityBundle* bundle, bool draw_intermediate = false);

	void iterate(const float& delta_time);

private:

	std::vector<Utility>* utilities;
	
	bool generate_platter = true;

	float prev_radius = NULL;

	//float containing_radius = 0.0f;
	Circle* containing_circle = nullptr;

	float U_old = 0.0f;
	bool last_rdp_same = false;
	float l_0 = 0.0f;

	float t = 0.0f;

	float unit_step = 1.0f;

	float time = 0.0f;

	void reset();
	void generate_initial_configuration();

	std::vector<vec2> calculate_force_vectors(float& total_force);
	vec2 calculate_force_vector(Circle& c);

	void apply_force_vectors(std::vector<vec2>& force_vectors);

	std::vector<float> calculate_system_potential(float& total_potential);
	float calculate_circle_potential(Circle& c);
};

#endif