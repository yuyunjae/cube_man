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
	glm::vec2 verCoord[6] = {
        glm::vec2(0.25f, 0.5f),
        glm::vec2(0.0f, 0.25f),
        glm::vec2(0.25f, 0.25f),
        glm::vec2(0.5f, 0.25f),
        glm::vec2(0.75f, 0.25f),
        glm::vec2(0.25f, 0.0f)
    };

    glm::vec2 moveWay[4] = {
        glm::vec2(0.0f, 0.25f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.25f, 0.0f),
        glm::vec2(0.25f, 0.25f)
    };
	void texturePos(int opt, int index);
	void quad(int a, int b, int c, int d, int opt, int &index);
	void makeQuad();
};
