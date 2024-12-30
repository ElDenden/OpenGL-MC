#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Light
{
public:
	Light(glm::vec3 position, glm::vec3 id) : m_Id(id), m_Pos(position)
	{

	}
	Light(){}

	glm::vec3 m_Id;
	glm::vec3 m_Pos;
	;
};