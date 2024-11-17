#pragma once

//#include <cmath>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

using namespace std;

class Cube {
public:
	// vector<glm::vec4> verts;
	// vector<glm::vec4> normals;
	// vector<glm::vec2> texCoords;
	glm::vec4 points[36];
	glm::vec4 normals[36];
	glm::vec2 texCoords[36];
	
	Cube() {
		makeQuad();
	};
	~Cube() {
	}
private:
	const int NumVertices = 36;
	glm::vec4 vertices[8] = {
		glm::vec4(-0.5, -0.5, 0.5, 1.0),
		glm::vec4(-0.5, 0.5, 0.5, 1.0),
		glm::vec4(0.5, 0.5, 0.5, 1.0),
		glm::vec4(0.5, -0.5, 0.5, 1.0),
		glm::vec4(-0.5, -0.5, -0.5, 1.0),
		glm::vec4(-0.5, 0.5, -0.5, 1.0),
		glm::vec4(0.5, 0.5, -0.5, 1.0),
		glm::vec4(0.5, -0.5, -0.5, 1.0)
	};
	void quad(int a, int b, int c, int d, int &Index);
	void makeQuad();
};
