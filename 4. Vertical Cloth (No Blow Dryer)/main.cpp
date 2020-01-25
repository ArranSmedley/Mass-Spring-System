#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"

// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Body.h"
#include "Particle.h"

using namespace glm;




void integrate(Particle& particle, double time, double dtime);

Gravity g = Gravity(glm::vec3(0.0f, -9.8f, 0.0f));


// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));

	Particle StaticParticles[10];

	for (int i = 0; i < 10; i++)
	{
		StaticParticles[i] = Particle();
		StaticParticles[i].setPos(vec3(i, 6, 0));
		StaticParticles[i].setVel(vec3(0, 0, 0));
		StaticParticles[i].setMass(1);

	}

	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.scale(glm::vec3(5.0f, 5.0f, 5.0f));
	plane.translate(glm::vec3(5.0f, -12.0f, 0.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.setShader(lambert);

	

	Hooke vert[10][10];
	Hooke horiz[9][10];
	Hooke diag1[9][10];
	Hooke diag2[9][10];

	Particle Particles[10][10];


	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			Particles[x][y] = Particle();
			Particles[x][y].setPos(vec3(x, 4 - y, 0));
			Particles[x][y].setVel(vec3(0, 0, 0));
			Particles[x][y].setMass(1);
			Particles[x][y].addForce(&g);

			//if y pos is = 0 then add a vertical force
			if (y == 0)
			{
				vert[x][y] = Hooke(&StaticParticles[x], &Particles[x][y], 9.8f, 4.0f, 1.0f);
				Particles[x][y].addForce(&vert[x][y]);
			}
			else
			{
				//Else take 1 off of the y value
				vert[x][y] = Hooke(&Particles[x][y - 1], &Particles[x][y], 9.8f, 4.0f, 1.0f);
				Particles[x][y].addForce(&vert[x][y]);
			}

			//If x value is less than 3 then add a horizontal force
			if (x < 3)
			{
				horiz[x][y] = Hooke(&Particles[x][y], &Particles[x + 1][y], 1.0f, 4.0f, 1.0f);
				Particles[x][y].addForce(&horiz[x][y]);

				//Same as the y value before but with the first diagonal force added giving it a cloth like effect
				if (y == 0)
				{
					diag1[x][y] = Hooke(&StaticParticles[x + 1], &Particles[x][y], 4.5f, 4.0f, 2);
					Particles[x][y].addForce(&diag1[x][y]);
				}
				else
				{
					diag1[x][y] = Hooke(&Particles[x + 1][y - 1], &Particles[x][y], 4.5f, 4.0f, 2);
					Particles[x][y].addForce(&diag1[x][y]);
				}
			}

			//If the x pos is greater than 0 and the y pos = 0 then add the 2nd diagonal force giving it the cloth effect
			if (x > 0)
			{
				if (y == 0)
				{
					diag2[x - 1][y] = Hooke(&StaticParticles[x - 1], &Particles[x][y], 4.5f, 4.0f, 2);
					Particles[x][y].addForce(&diag2[x - 1][y]);
				}
				else
				{
					diag2[x - 1][y - 1] = Hooke(&Particles[x - 1][y - 1], &Particles[x][y], 4.5f, 4.0f, 2);
					Particles[x][y].addForce(&diag2[x - 1][y - 1]);
				}
			}

		}
	}

	double t = 0.0;
	const double dt = 0.01;
	double currentTime = glfwGetTime();
	double accumulator = 0.0;



	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{

		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;
		accumulator += frameTime;


		while (accumulator >= dt)
		{

			for (int y = 0; y < 10; y++)
			{
				for (int x = 0; x < 10; x++)
				{
					integrate(Particles[x][y], t, dt);
				}
			}


			accumulator -= dt;
			t += dt;
		}


		app.doMovement(dt);
		/*
		**	SIMULATION
		*/



		/*

		**	RENDER
		*/
		// clear buffer
		app.clear();
		// draw groud plane
	//	app.draw(plane);
		// draw particles

		for (int i = 0; i < 10; i++)
		{
			app.draw(StaticParticles[i].getMesh());
	
		}

		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				app.draw(Particles[y][x].getMesh());
			}
		}

		// draw demo objects
		//app.draw(cube);
	//	app.draw(sphere);

		//app.draw(plane);
		app.display();

	}




	app.terminate();

	return EXIT_SUCCESS;
}

void integrate(Particle& particle, double time, double dtime)
{

	float m = particle.getMass();
	vec3 a = particle.applyForces(particle.getPos(), particle.getVel(), time, dtime) / m;
	particle.setAcc(a);
	vec3 v = particle.getVel() + (particle.getAcc() * dtime);
	vec3 r = particle.getPos() + (particle.getVel() * dtime);



	particle.setVel(v);
	particle.setPos(r);

}
