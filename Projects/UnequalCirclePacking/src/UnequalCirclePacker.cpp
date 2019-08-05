#include "UnequalCirclePacker.h"

//Independent Helper Methods

bool circles_intersect(const Circle& circle_1, const Circle& circle_2)
{
	float dist_between_centers = sqrt(pow(circle_1.x - circle_2.x, 2) + pow(circle_1.y - circle_2.y, 2));
	return dist_between_centers < circle_1.radius + circle_2.radius;
}

vec2 get_random_coordinates(float x_extemt, float y_extent)
{
	float x = 0.0f, y = 0.0f;

	float rand1 = ((float)rand()) / ((float)RAND_MAX);
	float rand2 = ((float)rand()) / ((float)RAND_MAX);

	x = (rand1*(2.0f*x_extemt)) - x_extemt;
	float y_range = sqrt((pow(y_extent, 2)) - pow(x, 2));
	y = (rand2*(2.0f*y_range)) - y_range;

	return vec2(x, y);
}

//------------------------------------------------------------------------------------------------------

UnequalCirclePacker::UnequalCirclePacker(UtilityBundle* bundle, bool draw_intermediate)
{
	srand(100);

	this->draw_intermediate = draw_intermediate;

	this->utilities = &(bundle->utilities);
	this->containing_circle = &(bundle->containing_circle);

	for (std::vector<Utility>::iterator u = this->utilities->begin(); u != this->utilities->end(); u++)
	{
		this->containing_radius += (*u).radius;
	}

	this->containing_radius /= 1.7f;
	*(this->containing_circle) = Circle(0, 0, this->containing_radius);
}

void UnequalCirclePacker::iterate(const float& delta_time)
{
	if (!done || !packed)
	{
		this->time += delta_time;

		//If a solution is found, reduce containing circle radius and try again
		if (packed)
		{
			this->prev_radius = this->containing_radius;
			this->containing_radius -= this->radius_stepdown_value;
			this->containing_circle->set_radius(this->containing_radius);

			this->reset();

			return;
		}

		//If we exceed our cutoff time without a solution being found, consider us done
		if (this->time > this->done_time_cutoff && !packed)
		{
			this->containing_radius = this->prev_radius;
			this->containing_circle->set_radius(this->containing_radius);

			this->done = true;

			this->reset();

			return;
		}

		//Generates a new random circle arrangement if commanded
		if (this->generate_platter)
		{
			generate_initial_configuration();
			this->generate_platter = false;
		}

		//Calculate system elastic potential
		std::vector<float> potentials;
		float total_potential = 0.0f;
		potentials = calculate_system_potential(total_potential);

		//Calculate the force vector for each circle in the system
		std::vector<vec2> force_vectors;
		float total_force = 0.0f;
		force_vectors = calculate_force_vectors(total_force);

		//Execute this block until we reach a sufficiently small unit step for our circle movement
		if (this->unit_step >= this->unit_step_cutoff)
		{
			//If we reach a sufficiently small system potential, consider the configuration packed
			if (total_potential < this->system_potential_cutoff)
			{
				this->packed = true;
				return;
			}

			//If the current system potential has moved to a higher potential state
			if (total_potential >= this->U_old)
			{
				//Scale down our unit step
				this->unit_step *= this->unit_step_stepdown_value;
			}

			this->U_old = total_potential;
			this->apply_force_vectors(force_vectors);

			return;
		}

		//--------------------------------------------------------------------------------

		std::vector<float>::iterator p = potentials.begin();
		std::vector<Utility>::iterator u = this->utilities->begin();

		Circle& c = (*u);

		//Find the circle in the configuration with the highest RDP (Relative Degree of Pain)
		//A metric to determine the circle experiencing the most force relative to its radius
		float highest_rdp = ((*p) / pow(c.radius, 2));
		int highest_rdp_index = 0;

		int i = 0;
		for (p++, u++, i++; p != potentials.end(), u != this->utilities->end(); p++, u++, i++)
		{
			c = (*u);

			float current_rdp = ((*p) / pow(c.radius, 2));
			if (current_rdp > highest_rdp)
			{
				highest_rdp = current_rdp;
				highest_rdp_index = i;
			}
		}

		//If the same circle is selected, toggle selection metric
		if (highest_rdp_index == this->l_0)
		{
			this->t += 1;
		}

		//If we're selecting the circle with the highest RDP
		if (this->t < 1)
		{
			vec2 coords = get_random_coordinates(this->containing_radius, this->containing_radius);

			c = utilities->at(highest_rdp_index);
			c.set_pos(coords.x, coords.y);

			this->l_0 = highest_rdp_index;
			this->unit_step = 1.0f;
		}
		//If we're selecting the circle with the lowest DP
		else if (t == 1)
		{
			p = potentials.begin();

			//Find the circle in the configuration with the low DP (Degree of Pain)
			//A metric to determine the circle experiencing the least force overall

			float lowest_dp = (*p);
			int lowest_dp_index = 0;

			int i = 0;
			for (p++, i++; p != potentials.end(); p++, i++)
			{
				float current_dp = (*p);
				if (current_dp < lowest_dp)
				{
					lowest_dp = current_dp;
					lowest_dp_index = i;
				}
			}

			vec2 coords = get_random_coordinates(this->containing_radius, this->containing_radius);

			c = utilities->at(lowest_dp_index);
			c.set_pos(coords.x, coords.y);

			this->t = 0.0f;
			this->l_0 = 0.0f;
			this->unit_step = 1.0f;
		}
	}
}

void UnequalCirclePacker::reset()
{
	this->U_old = 0.0f;
	this->l_0 = 0.0f;
	this->t = 0.0f;
	this->unit_step = 1.0f;

	this->time = 0.0f;

	this->packed = false;
	this->generate_platter = true;
}

void UnequalCirclePacker::generate_initial_configuration()
{
	std::vector<Utility>::iterator u;

	for (int i = 0; i < this->utilities->size(); i++)
	{
		Circle& c = this->utilities->at(i);

		vec2 coords = get_random_coordinates(this->containing_radius, this->containing_radius);
		c.set_pos(coords.x, coords.y);
	}
}

std::vector<vec2> UnequalCirclePacker::calculate_force_vectors(float& total_force)
{
	std::vector<vec2> force_vectors;

	for (std::vector<Utility>::iterator u = this->utilities->begin(); u != this->utilities->end(); u++)
	{
		Circle& current_circle = (*u);

		vec2 force_vector = calculate_force_vector(current_circle);

		float delta_x = force_vector.x * this->unit_step;
		float delta_y = force_vector.y * this->unit_step;

		force_vectors.push_back(vec2(delta_x, delta_y));
	}

	for (std::vector<vec2>::iterator f = force_vectors.begin(); f != force_vectors.end(); f++)
	{
		total_force += (*f).magnitude();
	}

	return force_vectors;
}

vec2 UnequalCirclePacker::calculate_force_vector(Circle& c)
{
	vec2 force_vector;

	Circle& target_circle = c;

	Circle& center_circle = (*containing_circle);

	float target_circle_dist_from_origin = sqrt(pow(target_circle.x, 2) + pow(target_circle.y, 2));

	if (target_circle_dist_from_origin + target_circle.radius > center_circle.radius)
	{
		float embedding_depth = target_circle_dist_from_origin + target_circle.radius - center_circle.radius;

		vec2 triangle_proportions;
		triangle_proportions.x += (center_circle.x - target_circle.x);
		triangle_proportions.y += (center_circle.y - target_circle.y);
		float triangle_hypotenuse = triangle_proportions.magnitude();
		force_vector.x = triangle_proportions.x / triangle_hypotenuse;
		force_vector.y = triangle_proportions.y / triangle_hypotenuse;
	}

	for (std::vector<Utility>::iterator u = this->utilities->begin(); u != this->utilities->end(); u++)
	{
		Circle& cur_circle = (*u);

		if (&cur_circle != &target_circle)
		{
			vec2 force_on_target;
			if (circles_intersect(target_circle, cur_circle))
			{
				float embedding_depth = target_circle.radius + cur_circle.radius - sqrt(pow(target_circle.x - cur_circle.x, 2) + pow(target_circle.y - cur_circle.y, 2));

				vec2 triangle_proportions;
				triangle_proportions.x += (target_circle.x - cur_circle.x);
				triangle_proportions.y += (target_circle.y - cur_circle.y);
				float triangle_hypotenuse = triangle_proportions.magnitude();

				force_on_target.x = triangle_proportions.x / triangle_hypotenuse;
				force_on_target.y = triangle_proportions.y / triangle_hypotenuse;
			}
			force_vector.x += force_on_target.x;
			force_vector.y += force_on_target.y;
		}
	}

	float force_vector_magnitude = sqrt(pow(force_vector.x, 2) + pow(force_vector.y, 2));

	if (force_vector_magnitude != 0)
	{
		force_vector.x /= force_vector_magnitude;
		force_vector.y /= force_vector_magnitude;
	}

	return force_vector;
}

void UnequalCirclePacker::apply_force_vectors(std::vector<vec2>& force_vectors)
{
	std::vector<Utility>::iterator u;
	std::vector<vec2>::iterator f;

	//Apply our unit step motion along the force vector for each circle
	for (u = this->utilities->begin(), f = force_vectors.begin(); u != this->utilities->end(), f != force_vectors.end(); u++, f++)
	{
		Circle& current_circle = (*u);
		vec2 current_force = (*f);

		current_circle.set_pos(current_circle.x + current_force.x, current_circle.y + current_force.y);
	}
}

std::vector<float> UnequalCirclePacker::calculate_system_potential(float& total_potential)
{
	std::vector<float> potentials;

	for (std::vector<Utility>::iterator u = this->utilities->begin(); u != this->utilities->end(); u++)
	{
		Circle& current_circle = (*u);

		float force_vector = calculate_circle_potential(current_circle);

		potentials.push_back(force_vector);
	}

	for (std::vector<float>::iterator f = potentials.begin(); f != potentials.end(); f++)
	{
		total_potential += (*f);
	}

	return potentials;
}

float UnequalCirclePacker::calculate_circle_potential(Circle& c)
{
	float circle_potential = 0.0f;

	Circle& target_circle = c;

	Circle& center_circle = (*containing_circle);

	float target_circle_dist_from_origin = sqrt(pow(target_circle.x, 2) + pow(target_circle.y, 2));

	if (target_circle_dist_from_origin + target_circle.radius > center_circle.radius)
	{
		float embedding_depth = target_circle_dist_from_origin + target_circle.radius - center_circle.radius;
		circle_potential += pow(embedding_depth, 2);
	}

	for (std::vector<Utility>::iterator u = this->utilities->begin(); u != this->utilities->end(); u++)
	{
		Circle& cur_circle = (*u);

		if (&cur_circle != &target_circle)
		{
			float interaction_potential = 0.0f;
			if (circles_intersect(target_circle, cur_circle))
			{
				float embedding_depth = target_circle.radius + cur_circle.radius - sqrt(pow(target_circle.x - cur_circle.x, 2) + pow(target_circle.y - cur_circle.y, 2));
				circle_potential += pow(embedding_depth, 2);
			}
		}
	}

	return circle_potential;
}
