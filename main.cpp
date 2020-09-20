#include <iostream>
#include <vector>
#include <math.h>

// Big G gravitational constant
#define G .0000000000667430



// definition of a 3-dimensional vector
struct Vector3
{
	long double x;
	long double y;
	long double z;
	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(long double x, long double y, long double z) : x(x), y(y), z(z)
	{
	}
	Vector3(long double magnitude, Vector3 direction)
	{
		x = direction.x * magnitude;
		y = direction.y * magnitude;
		z = direction.z * magnitude;
	}

	long double magnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 normalize()
	{
		Vector3 new_vector;
		long double magnitude = this->magnitude();
		new_vector.x = x / magnitude;
		new_vector.y = y / magnitude;
		new_vector.z = z / magnitude;
		return new_vector;
	}

	void display()
	{
		std::cout << "< " << x << ", " << y << ", " << z << " >"
				  << "\n";
	}
	Vector3 operator*(const long double scalar)
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}
	Vector3 operator/(const long double scalar)
	{
		return Vector3(x / scalar, y / scalar, z / scalar);
	}
	Vector3 operator+(const Vector3 new_vector)
	{
		return Vector3(x + new_vector.x, y + new_vector.y, z + new_vector.z);
	}
	Vector3 operator-(const Vector3 new_vector)
	{
		return Vector3(x - new_vector.x, y - new_vector.y, z - new_vector.z);
	}
};

struct Body
{

	Vector3 position;
	Vector3 momentum;
	long double mass;

	Body(Vector3 position, Vector3 momentum, long double mass) : position(position), momentum(momentum), mass(mass){};
	Body(Vector3 position, long double mass) : position(position), mass(mass)
	{
		momentum = Vector3(0, 0, 0);
	};
	Body(long double mass) : mass(mass)
	{
		position = Vector3(0, 0, 0);
		momentum = Vector3(0, 0, 0);
	};

	static Vector3 calculate_gravity(Body &body_1, Body &body_2);
};

// calculate the force of gravity acting on body one by body 2
Vector3 Body::calculate_gravity(Body &body_1, Body &body_2)
{
	Vector3 displacement_vector = (body_1.position - body_2.position);
	Vector3 unit_vector = displacement_vector.normalize();
	long double distance = displacement_vector.magnitude();
	long double magnitude = (G * body_1.mass * body_2.mass) / (distance * distance);
	return Vector3(magnitude, unit_vector * -1);
}

class Environment
{
private:
	std::vector<Body *> bodies;

public:
	void add_body(Body *body)
	{
		bodies.push_back(body);
	}

	// simulate a set timespan at a given resolution (ex 24 hours at 10 second intervals)
	void simulate(long double time_span, long double interval)
	{
		for (unsigned long long int i = 0; i < time_span / interval; i++)
		{
			step(interval);
		}
	}

	// simulate a set number of intervals at a given time resolution (ex 10 intervals of 30 seconds)
	void simulate_intervals(unsigned long long int iterations, long double interval)
	{
		for (unsigned long long int i = 0; i < iterations; i++)
		{
			step(interval);
		}
	}

	void step(long double time_step)
	{
		// step 1: calculate the net forces acting on all the objects
		// Note: This calculates forces inefficiently (I.E double calculates some forces)
		// But makes the code much cleaner.
		std::vector<Vector3> net_forces;
		for (int i = 0; i < bodies.size(); i++)
		{
			Vector3 current_force(0, 0, 0);
			// itterates all the other bodies and calculates what force they apply to bodies[i]
			for (int j = 0; j < bodies.size(); j++)
			{
				if (i == j)
				{
					continue;
				}
				current_force = current_force + Body::calculate_gravity(*bodies[i], *bodies[j]);
			}

			current_force.display();

			// add the net force acting on this body to the array of net forces
			net_forces.push_back(current_force);
		}

		// step 2: update the momentum of every object
		for (int i = 0; i < bodies.size(); i++)
		{
			bodies[i]->momentum = bodies[i]->momentum + net_forces[i] * time_step;
		}

		// step 3: update the position of every object
		for (int i = 0; i < bodies.size(); i++)
		{
			// calculate the average velocity as simply the final velocity
			Vector3 average_velocity = bodies[i]->momentum / bodies[i]->mass;
			bodies[i]->position = bodies[i]->position + average_velocity * time_step;
		}
	}
};

int main()
{
	// the position vectors for all the bodies in the problem
	Vector3 p1(0, 0, 0);
	Vector3 p2(0, 0, 115470);
	Vector3 p3(-100000, 0, -57735);
	Vector3 p4(100000, 0, -57735);


	// create those bodies and add their masses
	Body b1(p1, 5000);
	Body b2(p2, 5000);
	Body b3(p3, 5000);
	Body b4(p4, 5000);


	// create an environment
	Environment space;
	space.add_body(&b1);
	space.add_body(&b2);
	space.add_body(&b3);
	space.add_body(&b4);

	long long int time_span = 3600 * 24;
	time_span *= 365.25 * 100;


	// simulate 100 years at a resolution of 1 hour
	// space.simulate(time_span, 3600);
	space.simulate_intervals(1, 1);

	b1.position.display();
	b2.position.display();
	b3.position.display();
	b4.position.display();
}