#include <iostream>
#include <cmath>
#include "Force.h"
#include "Body.h"
#include "glm/ext.hpp"


glm::vec3 Force::apply(float mass, const glm::vec3& pos, const glm::vec3& vel)
{ return glm::vec3(0.0f); }
/* ** GRAVITY */
glm::vec3 Gravity::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) 
{ // complete. Should return the gravity force 
	return glm::vec3(0.0f, -9.8f, 0.0f);
}
	/* ** DRAG */
	glm::vec3 Drag::apply(float mass, const glm::vec3 & pos, const glm::vec3 & vel) { // complete. Should return the aerodynamic drag force 
		glm::vec3 drag = (1.225f * (normalize(vel) * normalize(vel)) * 1.05f * (-vel / -normalize(vel)) / 2);
		return drag;
	}

	/* ** HOOKE’S LAW */ 
	glm::vec3 Hooke::apply(float mass, const glm::vec3& pos, const glm::vec3& vel) { 

	
		glm::vec3 force;
	/*	glm::vec3 I = normalize(m_b1->getPos() - m_b2->getPos());

		glm::vec3 e = (m_b2->getPos() - m_b1->getPos()) / I;
		glm::vec3 v1 = glm::vec3(dot(e, m_b1->getVel()));
		glm::vec3 v2 = glm::vec3(dot(e, m_b2->getVel()));*/

		float l = glm::length(m_b2->getPos().x - m_b1->getPos().x);
		float e = glm::normalize(m_b2->getPos().x - m_b1->getPos().x);
		float v1 = glm::dot(e, m_b1->getVel().x);
		float v2 = glm::dot(e, m_b2->getVel().x);
		float springX = -m_ks * (m_rest - l);
		float dampX = m_kd * (v1 - v2);
		float forceX = springX - dampX;

		l = glm::length(m_b2->getPos().y - m_b1->getPos().y);
		e = glm::normalize(m_b2->getPos().y - m_b1->getPos().y);
		v1 = glm::dot(e, m_b1->getVel().y);
		v2 = glm::dot(e, m_b2->getVel().y);
		float springY = -m_ks * (m_rest - l);
		float dampY = m_kd * (v1 - v2);
		float forceY = springY - dampY;

		l = glm::length(m_b2->getPos().z - m_b1->getPos().z);
		e = glm::normalize(m_b2->getPos().z - m_b1->getPos().z);
		v1 = glm::dot(e, m_b1->getVel().z);
		v2 = glm::dot(e, m_b2->getVel().z);
		float springZ = -m_ks * (m_rest - l);
		float dampZ = m_kd * (v1 - v2);
		float forceZ = springZ - dampZ;

		force = glm::vec3(0, forceY, 0);

		return force;

		}