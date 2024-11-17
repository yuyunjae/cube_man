#include "initShader.h"
#include "sphere.h"
#include "cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "texture.hpp"
#include <string>

enum eShadeMode { NO_LIGHT, GOURAUD, PHONG, NUM_LIGHT_MODE };

glm::mat4 projectMat;
glm::mat4 viewMat;
glm::mat4 modelMat = glm::mat4(1.0f);

int shadeMode = NO_LIGHT;

GLuint projectMatrixID;
GLuint viewMatrixID;
GLuint modelMatrixID;
GLuint shadeModeID;
GLuint textureModeID;
GLuint TextureID;

typedef glm::vec4  color4;
typedef glm::vec4  point4;

Sphere sphere(50, 50);
Cube cube;

//----------------------------------------------------------------------------

// OpenGL initialization
GLuint program;
GLuint vao[2], vbo[2]; // (sphere, cube)
GLuint textures[5]; // texture 
const char* textureFiles[5] = {
	"image/pumpkin.bmp", // head
	"image/body.bmp", // body
	"image/arm.bmp", // arm
	"image/leather.bmp", // upperLeg
	"image/foot.bmp" // lowerLeg
};

void init() 
{
	// Load shaders and use the resulting shader program
	program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	if (!program) {
		std::cerr << "Shader program failed to initialize" << std::endl;
		exit(EXIT_FAILURE);
	}
	glUseProgram(program);

    glGenVertexArrays(2, vao); // (sphere, cube)
    glGenBuffers(2, vbo); // (sphere, cube)

    // sphere
    glBindVertexArray(vao[0]); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    int sphereVertSize = sizeof(sphere.verts[0]) * sphere.verts.size();
    int sphereNormalSize = sizeof(sphere.normals[0]) * sphere.normals.size();
    int sphereTexSize = sizeof(sphere.texCoords[0]) * sphere.texCoords.size();

    glBufferData(GL_ARRAY_BUFFER, sphereVertSize + sphereNormalSize + sphereTexSize, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sphereVertSize, sphere.verts.data());
    glBufferSubData(GL_ARRAY_BUFFER, sphereVertSize, sphereNormalSize, sphere.normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, sphereVertSize + sphereNormalSize, sphereTexSize, sphere.texCoords.data());

	// set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sphereVertSize));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sphereVertSize + sphereNormalSize));

	// cube
	glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

    int cubeVertSize = sizeof(cube.points);
    int cubeNormalSize = sizeof(cube.normals);
    int cubeTexSize = sizeof(cube.texCoords);

    glBufferData(GL_ARRAY_BUFFER, cubeVertSize + cubeNormalSize + cubeTexSize, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cubeVertSize, cube.points);
    glBufferSubData(GL_ARRAY_BUFFER, cubeVertSize, cubeNormalSize, cube.normals);
    glBufferSubData(GL_ARRAY_BUFFER, cubeVertSize + cubeNormalSize, cubeTexSize,cube.texCoords);

    // set up vertex arrays
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(cubeVertSize));

    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(cubeVertSize + cubeNormalSize));

    glBindVertexArray(0); // VAO 바인딩 해제


	projectMatrixID = glGetUniformLocation(program, "mProject");
	projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);

	viewMatrixID = glGetUniformLocation(program, "mView");
	viewMat = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);

	modelMatrixID = glGetUniformLocation(program, "mModel");
	modelMat = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);

	shadeModeID = glGetUniformLocation(program, "shadeMode");
	glUniform1i(shadeModeID, shadeMode);


	for (int i = 0; i < 5; ++i) {
		textures[i] = loadBMP_custom(textureFiles[i]);
	}
	//GLuint Texture = loadDDS("uvtemplate.DDS");

	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(program, "cubeTextures");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}


// element size
glm::vec3 bodySize = glm::vec3(0.3, 0.5, 0.4);
glm::vec3 headSize = glm::vec3(0.25, 0.25, 0.25);
glm::vec3 upperlegSize = glm::vec3(0.15, 0.3, 0.15); // upperleg, lowerleg
glm::vec3 armSize = glm::vec3(0.12, 0.3, 0.12); // arm, forearm


// standard position of parts
glm::vec3 bodyPos = glm::vec3(0, 0, 0);
glm::vec3 headPos = glm::vec3(0, (bodySize[1] + headSize[1]) / 2 + 0.1, 0);

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


void	drawHeadParts(glm::mat4& manMat, glm::vec3& partsPos, glm::vec3& partsSize, glm::vec3& upDownVec3) // head
{
	glm::mat4 headMat, pvmMat;

	glBindVertexArray(vao[0]); // sphere 

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(TextureID, 0);

	headMat = glm::translate(manMat, partsPos + upDownVec3);
	headMat = glm::rotate(headMat, glm::radians(90.0f), glm::vec3(1, 0, 0));
	headMat = glm::scale(headMat, partsSize);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &headMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, sphere.verts.size());
}

void	drawBodyParts(glm::mat4& manMat, glm::vec3& partsPos, glm::vec3& partsSize, glm::vec3& upDownVec3) // body
{
	glm::mat4 headMat, pvmMat;

	glBindVertexArray(vao[1]); // cube

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(TextureID, 0);

	headMat = glm::translate(manMat, partsPos + upDownVec3);
	headMat = glm::scale(headMat, partsSize);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &headMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTICES);
}

void	drawJointUpperParts(glm::mat4& manMat, int matIndex, glm::vec3& upDownVec3) // arm
{
	glm::mat4 topMat, bottomMat;
	float armAngle;
	float forearmAngle;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glUniform1i(TextureID, 0);

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

	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &topMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTICES);

	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &bottomMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTICES);
}


void	drawJointLowerParts(glm::mat4& manMat, int matIndex, glm::vec3& upDownVec3) // leg
{
	glm::mat4 topMat, bottomMat;
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glUniform1i(TextureID, 0);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &topMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTICES);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glUniform1i(TextureID, 0);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &bottomMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTICES);
}

void	drawMan(glm::mat4 manMat)
{
	glm::mat4 pvmMat;
	glm::vec3 upDownVec3 = glm::vec3(0, upDownMove[timeIndex] - (upDownMove[timeIndex] - upDownMove[(timeIndex + 1) % moveCount]) * (timeRatio - timeIndex * timeInterval) / timeInterval, 0);


	drawHeadParts(manMat, headPos, headSize, upDownVec3);
	drawBodyParts(manMat, bodyPos, bodySize, upDownVec3);


	for (int i = 0; i < 2; i++)
	{
		drawJointUpperParts(manMat, i, upDownVec3); // arm, forearm
		drawJointLowerParts(manMat, i, upDownVec3); // upperleg, lowerleg
	}
}


//----------------------------------------------------------------------------

void display(void)
{
	glm::mat4 worldMat;
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

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'l': case 'L':
		shadeMode = (++shadeMode % NUM_LIGHT_MODE);
		glUniform1i(shadeModeID, shadeMode);
		glutPostRedisplay();
		break;
	case '1':
		viewMat = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);
		break;
	case '2':
		viewMat = glm::lookAt(glm::vec3(2, 0.8, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);
		break;
	case '3':
		viewMat = glm::lookAt(glm::vec3(-2, -0.4, -0.4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);
		break;
	case '4':
		viewMat = glm::lookAt(glm::vec3(0, 0, -2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);
		break;
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

void resize(int w, int h)
{
	float ratio = (float)w / (float)h;
	glViewport(0, 0, w, h);

	projectMat = glm::perspective(glm::radians(65.0f), ratio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
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
