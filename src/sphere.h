#pragma once

//#include <cmath>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

using namespace std;

const float PI = glm::pi<float>();

class Sphere {
public:
	vector<glm::vec4> verts;
	vector<glm::vec4> normals;
	vector<glm::vec2> texCoords;

	Sphere() :Sphere(20, 20) {  }
	Sphere(int n) :Sphere(n, n) {  }
	Sphere(int nLongi, int nLati) { 
		makeUV(nLongi, nLati);
		computeNormals();
	};
	~Sphere() {
		verts.clear();
		vector<glm::vec4>().swap(verts);
		normals.clear();
		vector<glm::vec4>().swap(normals);
	}
private:
	const float PI = glm::pi<float>();
	int nLongitude;
	int nLatitude;
	void makeUV(int nLongi, int nLati);
	void computeNormals();
	void computeTexCoordQuad(glm::vec2 texcoord[4], int u, int v, int u2, int v2);
} ;


