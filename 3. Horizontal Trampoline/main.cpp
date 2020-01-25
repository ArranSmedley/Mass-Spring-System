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
	Application::camera.setCameraPosition(glm::vec3(3.0f, 0.0f, 20.0f));

	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.scale(glm::vec3(5.0f, 5.0f, 5.0f));
	plane.translate(glm::vec3(5.0f, -6.0f, 5.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.setShader(lambert);

	Particle pmove[10][10];


	Particle StaticParticles[10];

	


	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			pmove[x][y] = Particle();
			pmove[x][y].setPos(vec3(x, 0, y));
			pmove[x][y].setVel(vec3(0, 0, 0));
			pmove[x][y].setMass(1);

			if (x > 0 && y > 0 && x < 9 && y < 9)
			{

					pmove[x][y].addForce(&g);

					//Adding a force to every particle in the 10x10 area, x forces and y forces
					Hooke* h = new Hooke(&pmove[x][y], &pmove[x][y - 1], 20.0f, 4.0f, 1.0f);
					pmove[x][y].addForce(h);
					Hooke* h1 = new Hooke(&pmove[x][y], &pmove[x][y + 1], 20.0f, 4.0f, 1.0f);
					pmove[x][y].addForce(h1);
					Hooke* h2 = new Hooke(&pmove[x][y], &pmove[x - 1][y], 20.0f, 4.0f, 1.0f);
					pmove[x][y].addForce(h2);
					Hooke* h3 = new Hooke(&pmove[x][y], &pmove[x + 1][y], 20.0f, 4.0f, 1.0f);
					pmove[x][y].addForce(h3);

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
					integrate(pmove[x][y], t, dt);
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


		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				app.draw(pmove[x][y].getMesh());
			}
		}

		// draw demo objects
		//app.draw(cube);
	//	app.draw(sphere);

	//	app.draw(plane);
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
