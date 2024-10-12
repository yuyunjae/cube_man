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

float rotAngle = 0.0f;

int isDrawingMan = false;
int isDrawingCar = false;

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

//void drawCar(glm::mat4 carMat)
//{
//	glm::mat4 modelMat, pvmMat;
//	glm::vec3 wheelPos[4];
//
//	wheelPos[0] = glm::vec3(0.3, 0.24, -0.1); // rear right
//	wheelPos[1] = glm::vec3(0.3, -0.24, -0.1); // rear left
//	wheelPos[2] = glm::vec3(-0.3, 0.24, -0.1); // front right
//	wheelPos[3] = glm::vec3(-0.3, -0.24, -0.1); // front left
//
//	// car body
//	modelMat = glm::scale(carMat, glm::vec3(1, 0.6, 0.2));
//	pvmMat = projectMat * viewMat * modelMat;
//	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
//	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
//
//	// car top
//	modelMat = glm::translate(carMat, glm::vec3(0, 0, 0.2));  //P*V*C*T*S*v
//	modelMat = glm::scale(modelMat, glm::vec3(0.5, 0.6, 0.2));
//	pvmMat = projectMat * viewMat * modelMat;
//	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
//	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
//
//	// car wheel
//	for (int i = 0; i < 4; i++)
//	{
//		modelMat = glm::translate(carMat, wheelPos[i]);  //P*V*C*T*S*v
//		modelMat = glm::scale(modelMat, glm::vec3(0.2, 0.1, 0.2));
//		modelMat = glm::rotate(modelMat, -rotAngle*50.0f, glm::vec3(0, 1, 0));
//		pvmMat = projectMat * viewMat * modelMat;
//		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
//		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
//	}
//}

// element size
glm::vec3 bodySize = glm::vec3(0.3, 0.5, 0.4);
glm::vec3 headSize = glm::vec3(0.25, 0.25, 0.25);
glm::vec3 legSize = glm::vec3(0.15, 0.3, 0.15);
glm::vec3 armSize = glm::vec3(0.12, 0.3, 0.12);

// move period
int period = 1000;

// up-down move
float upDownMove[] = { 0.2f, 0.0f, 0.2f, 0.0f };

// arm move
float armLeft[] = { glm::radians(45.0f), 0.0f, glm::radians(45.0f), 0.0f };
float armRight[] = { glm::radians(-45.0f), 0.0f, glm::radians(-45.0f), 0.0f };

// hand move
float handLeft[] = { glm::radians(60.0f), 0.0f, glm::radians(60.0f), 0.0f };
float handRight[] = { glm::radians(-60.0f), 0.0f, glm::radians(-60.0f), 0.0f };

// leg move
float legLeft[] = { glm::radians(45.0f), 0.0f, glm::radians(45.0f), 0.0f };
float legRight[] = { glm::radians(-45.0f), 0.0f, glm::radians(-45.0f), 0.0f };

// foot move
float footLeft[] = { glm::radians(60.0f), 0.0f, glm::radians(60.0f), 0.0f };
float footRight[] = { glm::radians(-60.0f), 0.0f, glm::radians(-60.0f), 0.0f };

int timeRatio = 0;
int timeIndex = 0;


// standard position of parts
glm::vec3 bodyPos = glm::vec3(0, 0, 0);
glm::vec3 headPos = glm::vec3(0, (bodySize[1] + headSize[1]) / 2, 0);

glm::vec3 armPos[2] = 
{
	glm::vec3(0, (bodySize[1] - armSize[1]) / 2, (bodySize[2] + armSize[2]) / (-2)),  // right arm
	glm::vec3(0, (bodySize[1] - armSize[1]) / 2, (bodySize[2] + armSize[2]) / 2)   // left arm
};

glm::vec3 handPos[2] =
{
	glm::vec3(0, (bodySize[1] - 3 * armSize[1]) / 2, (bodySize[2] + armSize[2]) / (-2)),  // right hand
	glm::vec3(0, (bodySize[1] - 3 * armSize[1]) / 2, (bodySize[2] + armSize[2]) / 2) // left hand
};

glm::vec3 legPos[2] =
{
	 glm::vec3(0, (bodySize[1] + legSize[1]) / (-2), legSize[2] / (-2)), // right leg
	 glm::vec3(0, (bodySize[1] + legSize[1]) / (-2), legSize[2] / 2)   // left leg
};

glm::vec3 footPos[2] =
{
	glm::vec3(0, (bodySize[1] + 3 * legSize[1]) / (-2), legSize[2] / (-2)),  // right foot
	glm::vec3(0, (bodySize[1] + 3 * legSize[1]) / (-2), legSize[2] / 2) // left foot
};

// joint (body-arm | arm-hand || body-leg | leg-foot)
glm::vec3 moveUpJointPos = glm::vec3(0, armSize[1] / 2, 0);
glm::vec3 moveDownJointPos = glm::vec3(0, armSize[1] / (-2), 0);


void	drawParts(glm::mat4 &manMat, glm::vec3 &partsPos, glm::vec3 &partsSize)
{
	glm::mat4 modelMat, pvmMat;
	
	modelMat = glm::translate(manMat, partsPos);
	modelMat = glm::scale(modelMat, partsSize);
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void	drawJointParts(glm::mat4& manMat, int matIndex)
{
	glm::mat4 topMat, bottomMat, pvmMat;
	float moveAngle = armLeft[timeIndex] * timeRatio / 250; // ¹Ì¿Ï

	topMat = glm::translate(manMat, armPos[matIndex]);
	topMat = glm::translate(topMat, moveUpJointPos);
	topMat = glm::rotate(topMat, -rotAngle, glm::vec3(0, 0, 1));
	topMat = glm::translate(topMat, moveDownJointPos);
	topMat = glm::scale(topMat, armSize);

	bottomMat = glm::translate(manMat, armPos[matIndex]);
	bottomMat = glm::translate(bottomMat, moveUpJointPos);
	bottomMat = glm::rotate(bottomMat, -rotAngle, glm::vec3(0, 0, 1)); // also rotate bottom parts
	bottomMat = glm::translate(bottomMat, glm::vec3(0, -armSize[1], 0)); // attach arm-hand
	bottomMat = glm::rotate(bottomMat, -rotAngle, glm::vec3(0, 0, 1));
	bottomMat = glm::translate(bottomMat, moveDownJointPos);
	bottomMat = glm::scale(bottomMat, armSize);

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

	drawParts(manMat, bodyPos, bodySize);
	drawParts(manMat, headPos, headSize);

	// leg
	for (int i = 0; i < 2; i++)
	{
		modelMat = glm::translate(manMat, legPos[i]);
		modelMat = glm::scale(modelMat, legSize);
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

	// foot
	for (int i = 0; i < 2; i++)
	{
		modelMat = glm::translate(manMat, footPos[i]);
		modelMat = glm::scale(modelMat, legSize);
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

	for (int i = 0; i < 2; i++)
	{
		drawJointParts(manMat, i);
	}

	//// hand
	//for (int i = 0; i < 2; i++)
	//{
	//	modelMat = glm::translate(manMat, handPos[i]);
	//	modelMat = glm::translate(modelMat, moveUpJointPos);
	//	modelMat = glm::rotate(modelMat, -rotAngle, glm::vec3(0, 0, 1));
	//	modelMat = glm::translate(modelMat, moveDownJointPos);
	//	modelMat = glm::scale(modelMat, armSize);
	//	pvmMat = projectMat * viewMat * modelMat;
	//	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	//	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	//}

	//// arm
	//for (int i = 0; i < 2; i++)
	//{
	//	modelMat = glm::translate(manMat, armPos[i]);
	//	modelMat = glm::translate(modelMat, moveUpJointPos);
	//	modelMat = glm::rotate(modelMat, -rotAngle, glm::vec3(0, 0, 1));
	//	modelMat = glm::translate(modelMat, moveDownJointPos);
	//	modelMat = glm::scale(modelMat, armSize);
	//	pvmMat = projectMat * viewMat * modelMat;
	//	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	//	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	//}
}


void display(void)
{
	glm::mat4 worldMat, pvmMat;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//worldMat = glm::rotate(glm::mat4(1.0f), rotAngle, glm::vec3(1.0f, 1.0f, 0.0f));
	worldMat = glm::mat4(1.0f);


	if (isDrawingMan)
		drawMan(worldMat);
	//else if (isDrawingCar)
	//{
	//	drawCar(worldMat);
	//}
	else
	{
		pvmMat = projectMat * viewMat * worldMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

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
		rotAngle += glm::radians(t*360.0f / 10000.0f);
		timeRatio = (timeRatio + (int)t) % period;
		timeIndex = timeRatio % 4;
		
		prevTime = currTime;
		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c': case 'C':
		isDrawingCar = !isDrawingCar;
		break;
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'm': case 'M':
		isDrawingMan = !isDrawingMan;
		break;
	case '1':
		viewMat = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case '2':
		viewMat = glm::lookAt(glm::vec3(2, 0.4, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case '3':
		viewMat = glm::lookAt(glm::vec3(2, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case '4':
		viewMat = glm::lookAt(glm::vec3(1, 0.8, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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
	glutCreateWindow("Color Car");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
