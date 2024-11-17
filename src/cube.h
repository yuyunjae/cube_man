#pragma once

//#include <cmath>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

using namespace std;

#define NUMVERTICES 36

class Cube {
public:
	// vector<glm::vec4> verts;
	// vector<glm::vec4> normals;
	// vector<glm::vec2> texCoords;
	glm::vec4 points[NUMVERTICES];
	glm::vec4 normals[NUMVERTICES];
	glm::vec2 texCoords[NUMVERTICES];

	Cube() {
		makeQuad();
	};
	~Cube() {
	}
private:
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
