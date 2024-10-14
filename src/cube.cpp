//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

glm::mat4 projectMat;
glm::mat4 viewMat;

GLuint pvmMatrixID;

typedef glm::vec4  color4;
typedef glm::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

// RGBA colors
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(0.0, 1.0, 1.0, 1.0),   // cyan
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 1.0, 1.0, 1.0)  // white
};


//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++; 
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d];  Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	colorcube();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	pvmMatrixID = glGetUniformLocation(program, "mPVM");

	projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
	viewMat = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

// element size
glm::vec3 bodySize = glm::vec3(0.3, 0.5, 0.4);
glm::vec3 headSize = glm::vec3(0.25, 0.25, 0.25);
glm::vec3 upperlegSize = glm::vec3(0.15, 0.3, 0.15); // upperleg, lowerleg
glm::vec3 armSize = glm::vec3(0.12, 0.3, 0.12); // arm, forearm


// standard position of parts
glm::vec3 bodyPos = glm::vec3(0, 0, 0);
glm::vec3 headPos = glm::vec3(0, (bodySize[1] + headSize[1]) / 2, 0);

glm::vec3 armPos[2] = 
{
	glm::vec3(0, (bodySize[1] - armSize[1]) / 2, (bodySize[2] + armSize[2]) / (-2)),  // right arm
	glm::vec3(0, (bodySize[1] - armSize[1]) / 2, (bodySize[2] + armSize[2]) / 2)   // left arm
};

glm::vec3 forearmPos[2] =
{
	glm::vec3(0, (bodySize[1] - 3 * armSize[1]) / 2, (bodySize[2] + armSize[2]) / (-2)),  // right forearm
	glm::vec3(0, (bodySize[1] - 3 * armSize[1]) / 2, (bodySize[2] + armSize[2]) / 2) // left forearm
};

glm::vec3 upperlegPos[2] =
{
	 glm::vec3(0, (bodySize[1] + upperlegSize[1]) / (-2), upperlegSize[2] / (-2)), // right upperleg
	 glm::vec3(0, (bodySize[1] + upperlegSize[1]) / (-2), upperlegSize[2] / 2)   // left upperleg
};

glm::vec3 lowerlegPos[2] =
{
	glm::vec3(0, (bodySize[1] + 3 * upperlegSize[1]) / (-2), upperlegSize[2] / (-2)),  // right lowerleg
	glm::vec3(0, (bodySize[1] + 3 * upperlegSize[1]) / (-2), upperlegSize[2] / 2) // left lowerleg
};

// joint (body-arm | arm-forearm || body-upperleg | upperleg-lowerleg)
glm::vec3 moveUpJointPos = glm::vec3(0, armSize[1] / 2, 0);
glm::vec3 moveDownJointPos = glm::vec3(0, armSize[1] / (-2), 0);


// move period
int period = 800;
int	moveCount = 8;
int timeInterval = period / moveCount;

// time
float timeRatio = 0.2f;
int timeIndex = 0;


// up-down move // 8 level
float upDownMove[] = { 0.1f, 0.05f, 0.0f, 0.05f, 0.1f, 0.05f, 0.0f, 0.05f };

// arm move
float armLeft[] = { glm::radians(45.0f), glm::radians(32.0f), 0.0f, glm::radians(-32.0f), glm::radians(-45.0f), glm::radians(-32.0f), 0.0f, glm::radians(32.0f) };
float armRight[] = { glm::radians(-45.0f), glm::radians(-32.0f), 0.0f, glm::radians(32.0f), glm::radians(45.0f), glm::radians(32.0f), 0.0f, glm::radians(-32.0f) };

// forearm move
float forearmLeft[] = { glm::radians(-25.0f), glm::radians(-35.0f), glm::radians(-60.0f), glm::radians(-85.0f), glm::radians(-90.0f), glm::radians(-85.0f), glm::radians(-60.0f), glm::radians(-35.0f) };
float forearmRight[] = { glm::radians(-90.0f), glm::radians(-85.0f), glm::radians(-60.0f), glm::radians(-35.0f), glm::radians(-25.0f), glm::radians(-35.0f), glm::radians(-60.0f), glm::radians(-85.0f) };

// upperleg move
float upperlegLeft[] = { glm::radians(-60.0f), glm::radians(-40.0f), 0.0f, glm::radians(40.0f), glm::radians(60.0f), glm::radians(40.0f), 0.0f, glm::radians(-40.0f) };
float upperlegRight[] = { glm::radians(60.0f), glm::radians(40.0f), 0.0f, glm::radians(-40.0f), glm::radians(-60.0f), glm::radians(-40.0f), 0.0f, glm::radians(40.0f) };

// lowerleg move
float lowerlegLeft[] = { glm::radians(45.0f), glm::radians(30.0f), 0.0f, glm::radians(40.0f), glm::radians(50.0f), glm::radians(40.0f), 0.0f, glm::radians(30.0f) };
float lowerlegRight[] = { glm::radians(50.0f), glm::radians(40.0f), 0.0f, glm::radians(30.0f), glm::radians(45.0f), glm::radians(30.0f), 0.0f, glm::radians(40.0f) };


void	drawParts(glm::mat4 &manMat, glm::vec3 &partsPos, glm::vec3 &partsSize, glm::vec3 &upDownVec3)
{
	glm::mat4 modelMat, pvmMat;
	
	modelMat = glm::translate(manMat, partsPos + upDownVec3);
	modelMat = glm::scale(modelMat, partsSize);
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void	drawJointUpperParts(glm::mat4& manMat, int matIndex, glm::vec3& upDownVec3)
{
	glm::mat4 topMat, bottomMat, pvmMat;
	float armAngle;
	float forearmAngle;
	if (matIndex)
	{
		armAngle = armLeft[timeIndex] - (armLeft[timeIndex] - armLeft[(timeIndex + 1) % moveCount]) * abs(timeRatio - timeIndex * timeInterval) / timeInterval;
		forearmAngle = forearmLeft[timeIndex] - (forearmLeft[timeIndex] - forearmLeft[(timeIndex + 1) % moveCount]) * abs(timeRatio - timeIndex * timeInterval) / timeInterval;
	}
	else
	{
		armAngle = armRight[timeIndex] - (armRight[timeIndex] - armRight[(timeIndex + 1) % moveCount]) * abs(timeRatio - timeIndex * timeInterval) / timeInterval;
		forearmAngle = forearmRight[timeIndex] - (forearmRight[timeIndex] - forearmRight[(timeIndex + 1) % moveCount]) * abs(timeRatio - timeIndex * timeInterval) / timeInterval;
	}

	topMat = glm::translate(manMat, armPos[matIndex] + moveUpJointPos + upDownVec3);
	topMat = glm::rotate(topMat, armAngle, glm::vec3(0, 0, 1));
	topMat = glm::translate(topMat, moveDownJointPos);
	topMat = glm::scale(topMat, armSize);

	bottomMat = glm::translate(manMat, armPos[matIndex] + moveUpJointPos + upDownVec3);
	bottomMat = glm::rotate(bottomMat, armAngle, glm::vec3(0, 0, 1)); // also rotate bottom parts
	bottomMat = glm::translate(bottomMat, glm::vec3(0, -armSize[1], 0)); // attach arm-forearm
	bottomMat = glm::rotate(bottomMat, forearmAngle, glm::vec3(0, 0, 1));
	bottomMat = glm::translate(bottomMat, moveDownJointPos);
	bottomMat = glm::scale(bottomMat, armSize);

	pvmMat = projectMat * viewMat * topMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	pvmMat = projectMat * viewMat * bottomMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}


void	drawJointLowerParts(glm::mat4& manMat, int matIndex, glm::vec3& upDownVec3)
{
	glm::mat4 topMat, bottomMat, pvmMat;
	float upperlegAngle;
	float lowerlegAngle;
	if (matIndex)
	{
		upperlegAngle = upperlegLeft[timeIndex] - (upperlegLeft[timeIndex] - upperlegLeft[(timeIndex + 1) % moveCount]) * abs(timeRatio - timeIndex * timeInterval) / timeInterval;
		lowerlegAngle = lowerlegLeft[timeIndex] - (lowerlegLeft[timeIndex] - lowerlegLeft[(timeIndex + 1) % moveCount]) * abs(timeRatio - timeIndex * timeInterval) / timeInterval;
	}
	else
	{
		upperlegAngle = upperlegRight[timeIndex] - (upperlegRight[timeIndex] - upperlegRight[(timeIndex + 1) % moveCount]) * abs(timeRatio - timeIndex * timeInterval) / timeInterval;
		lowerlegAngle = lowerlegRight[timeIndex] - (lowerlegRight[timeIndex] - lowerlegRight[(timeIndex + 1) % moveCount]) * abs(timeRatio - timeIndex * timeInterval) / timeInterval;
	}

	topMat = glm::translate(manMat, upperlegPos[matIndex] + moveUpJointPos + upDownVec3);
	topMat = glm::rotate(topMat, upperlegAngle, glm::vec3(0, 0, 1));
	topMat = glm::translate(topMat, moveDownJointPos);
	topMat = glm::scale(topMat, upperlegSize);

	bottomMat = glm::translate(manMat, upperlegPos[matIndex] + moveUpJointPos + upDownVec3);
	bottomMat = glm::rotate(bottomMat, upperlegAngle, glm::vec3(0, 0, 1));
	bottomMat = glm::translate(bottomMat, glm::vec3(0, -upperlegSize[1], 0));
	bottomMat = glm::rotate(bottomMat, lowerlegAngle, glm::vec3(0, 0, 1));
	bottomMat = glm::translate(bottomMat, moveDownJointPos);
	bottomMat = glm::scale(bottomMat, upperlegSize);

	pvmMat = projectMat * viewMat * topMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	pvmMat = projectMat * viewMat * bottomMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void	drawMan(glm::mat4 manMat)
{
	glm::mat4 modelMat, pvmMat;
	glm::vec3 upDownVec3 = glm::vec3(0, upDownMove[timeIndex] - (upDownMove[timeIndex] - upDownMove[(timeIndex + 1) % moveCount]) * (timeRatio - timeIndex * timeInterval) / timeInterval, 0);

	drawParts(manMat, bodyPos, bodySize, upDownVec3);
	drawParts(manMat, headPos, headSize, upDownVec3);


	for (int i = 0; i < 2; i++)
	{
		drawJointUpperParts(manMat, i, upDownVec3); // arm, forearm
		drawJointLowerParts(manMat, i, upDownVec3); // upperleg, lowerleg
	}
}


void display(void)
{
	glm::mat4 worldMat, pvmMat;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	worldMat = glm::mat4(1.0f);
	drawMan(worldMat);

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idle()
{
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int currTime = glutGet(GLUT_ELAPSED_TIME);

	if (abs(currTime - prevTime) >= 20)
	{
		float t = abs(currTime - prevTime);
		timeRatio += t;
		while (timeRatio >= period)
			timeRatio -= period;
		timeIndex = (int)(timeRatio) / timeInterval;
		
		prevTime = currTime;
		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:  // Escape key (Esc)
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case '1':
		viewMat = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case '2':
		viewMat = glm::lookAt(glm::vec3(2, 0.8, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case '3':
		viewMat = glm::lookAt(glm::vec3(-2, -0.4, -0.4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	}
}

//----------------------------------------------------------------------------

void resize(int w, int h)
{
	float ratio = (float)w / (float)h;
	glViewport(0, 0, w, h);

	projectMat = glm::perspective(glm::radians(65.0f), ratio, 0.1f, 100.0f);

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Cube Man");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
