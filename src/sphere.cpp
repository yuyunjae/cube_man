#include "sphere.h"

void Sphere::makeUV(int nLongi, int nLati)
{
	int nverts = nLongi*nLati;
	float radius = 1;
	vector<glm::vec4> vertList;

	nLongitude = nLongi;
	nLatitude = nLati;
	// make vertex list
	for (int v = 0; v < nLati + 1; v++)
	{
		for (int u = 0; u < nLongi; u++)
		{
			float theta = 2 * PI * u / nLongi; // longitude
			float phi = PI * v / nLati;  // latitude
			float x = glm::sin(phi) * glm::cos(theta) * radius;
			float y = glm::sin(phi) * glm::sin(theta) * radius;
			float z = glm::cos(phi) * radius;
			vertList.push_back(glm::vec4(x, y, z, 1));
		}
	}

	// make triangles
	for (int v = 0; v < nLati; v++)
	{
		for (int u = 0; u < nLongi; u++)
		{
			int u2 = (u + 1) % nLongi, v2 = v + 1;
			// v0=(u, v)    v1=(u2, v)   <= quadangle
			// v2=(u, v2)   v3=(u2, v2)

			// triangle (u, v), (u, v2), (u2, v2)
			verts.push_back(vertList[u + v * nLongi]);  // v0
			verts.push_back(vertList[u + v2 * nLongi]);  // v2
			verts.push_back(vertList[u2 + v2 * nLongi]);  // v3

			// triangle (u, v), (u2, v2), (u2, v)
			verts.push_back(vertList[u + v * nLongi]);  // v0
			verts.push_back(vertList[u2 + v2 * nLongi]);  // v3
			verts.push_back(vertList[u2 + v * nLongi]);  // v1

			glm::vec2 texcoord[4];
			computeTexCoordQuad(texcoord, u, v, u2, v2);
			texCoords.push_back(texcoord[0]);
			texCoords.push_back(texcoord[2]);
			texCoords.push_back(texcoord[3]);

			texCoords.push_back(texcoord[0]);
			texCoords.push_back(texcoord[3]);
			texCoords.push_back(texcoord[1]);
		}
	}
}


void Sphere::computeNormals()
{
	for (int i = 0; i < verts.size(); i++)
	{
		glm::vec4 n;
		for (int k = 0; k < 3; k++)
		{
			n[k] = verts[i][k];
		}
		n[3] = 0.0;
		glm::normalize(n);
		normals.push_back(n);
	}
}

void Sphere::computeTexCoordQuad(glm::vec2 texcoord[4], int u, int v, int u2, int v2)
{
	const int U = 0, V = 1;
	// v0=(u, v)    v1=(u2, v)   <= quadangle
	// v2=(u, v2)   v3=(u2, v2)

	texcoord[0][U] = texcoord[2][U] = (float)u / nLongitude;
	texcoord[1][U] = texcoord[3][U] = (float)u2 / nLongitude;

	texcoord[0][V] = texcoord[1][V] = (float)v / nLatitude;
	texcoord[2][V] = texcoord[3][V] = (float)v2 / nLatitude;

	if (u2 == 0) // last column
	{
		texcoord[1][U] = texcoord[3][U] = 1.0;
	}
	//texcoord[u] = atan2(normals[y], normals[x]) / (2 * PI) + 0.5;
	//texcoord[v] = acos(normals[z]/sqrt(length(normal))) / PI;
}


/*
void Icosahedron(Mesh &mesh)
{
	const double t = (1.0 + std::sqrt(5.0)) / 2.0;

	// Vertices
	mesh.vertices.emplace_back(normalize(Vector3(-1.0,  t, 0.0)));
	mesh.vertices.emplace_back(normalize(Vector3( 1.0,  t, 0.0)));
	mesh.vertices.emplace_back(normalize(Vector3(-1.0, -t, 0.0)));
	mesh.vertices.emplace_back(normalize(Vector3( 1.0, -t, 0.0)));
	mesh.vertices.emplace_back(normalize(Vector3(0.0, -1.0,  t)));
	mesh.vertices.emplace_back(normalize(Vector3(0.0,  1.0,  t)));
	mesh.vertices.emplace_back(normalize(Vector3(0.0, -1.0, -t)));
	mesh.vertices.emplace_back(normalize(Vector3(0.0,  1.0, -t)));
	mesh.vertices.emplace_back(normalize(Vector3( t, 0.0, -1.0)));
	mesh.vertices.emplace_back(normalize(Vector3( t, 0.0,  1.0)));
	mesh.vertices.emplace_back(normalize(Vector3(-t, 0.0, -1.0)));
	mesh.vertices.emplace_back(normalize(Vector3(-t, 0.0,  1.0)));

	// Faces
	mesh.addTriangle(0, 11, 5);
	mesh.addTriangle(0, 5, 1);
	mesh.addTriangle(0, 1, 7);
	mesh.addTriangle(0, 7, 10);
	mesh.addTriangle(0, 10, 11);
	mesh.addTriangle(1, 5, 9);
	mesh.addTriangle(5, 11, 4);
	mesh.addTriangle(11, 10, 2);
	mesh.addTriangle(10, 7, 6);
	mesh.addTriangle(7, 1, 8);
	mesh.addTriangle(3, 9, 4);
	mesh.addTriangle(3, 4, 2);
	mesh.addTriangle(3, 2, 6);
	mesh.addTriangle(3, 6, 8);
	mesh.addTriangle(3, 8, 9);
	mesh.addTriangle(4, 9, 5);
	mesh.addTriangle(2, 4, 11);
	mesh.addTriangle(6, 2, 10);
	mesh.addTriangle(8, 6, 7);
	mesh.addTriangle(9, 8, 1);
}

struct Edge
{
	uint32_t v0;
	uint32_t v1;

	Edge(uint32_t v0, uint32_t v1)
		: v0(v0 < v1 ? v0 : v1)
		, v1(v0 < v1 ? v1 : v0)
	{
	}

	bool operator <(const Edge &rhs) const
	{
		return v0 < rhs.v0 || (v0 == rhs.v0 && v1 < rhs.v1);
	}
};

uint32_t subdivideEdge(uint32_t f0, uint32_t f1, const Vector3 &v0, const Vector3 &v1, Mesh &io_mesh, std::map<Edge, uint32_t> &io_divisions)
{
	const Edge edge(f0, f1);
	auto it = io_divisions.find(edge);
	if (it != io_divisions.end())
	{
		return it->second;
	}

	const Vector3 v = normalize(Vector3(0.5) * (v0 + v1));
	const uint32_t f = io_mesh.vertices.size();
	io_mesh.vertices.emplace_back(v);
	io_divisions.emplace(edge, f);
	return f;
}

void SubdivideMesh(const Mesh &meshIn, Mesh &meshOut)
{
	meshOut.vertices = meshIn.vertices;

	std::map<Edge, uint32_t> divisions; // Edge -> new vertex

	for (uint32_t i = 0; i < meshIn.triangleCount(); ++i)
	{
		const uint32_t f0 = meshIn.triangles[i * 3];
		const uint32_t f1 = meshIn.triangles[i * 3 + 1];
		const uint32_t f2 = meshIn.triangles[i * 3 + 2];

		const Vector3 v0 = meshIn.vertices[f0];
		const Vector3 v1 = meshIn.vertices[f1];
		const Vector3 v2 = meshIn.vertices[f2];

		const uint32_t f3 = subdivideEdge(f0, f1, v0, v1, meshOut, divisions);
		const uint32_t f4 = subdivideEdge(f1, f2, v1, v2, meshOut, divisions);
		const uint32_t f5 = subdivideEdge(f2, f0, v2, v0, meshOut, divisions);

		meshOut.addTriangle(f0, f3, f5);
		meshOut.addTriangle(f3, f1, f4);
		meshOut.addTriangle(f4, f2, f5);
		meshOut.addTriangle(f3, f4, f5);
	}
}
*/