#include "cube.h"

void Cube::texturePos(int opt, int index) {
    if (opt == 0 || opt == 5) {
        texCoords[index] = verCoord[opt] + moveWay[3]; index++;
        texCoords[index] = verCoord[opt] + moveWay[0]; index++;
        texCoords[index] = verCoord[opt] + moveWay[1]; index++;
        texCoords[index] = verCoord[opt] + moveWay[3]; index++;
        texCoords[index] = verCoord[opt] + moveWay[1]; index++;
        texCoords[index] = verCoord[opt] + moveWay[2]; index++;
    } else {
        texCoords[index] = verCoord[opt] + moveWay[0]; index++;
        texCoords[index] = verCoord[opt] + moveWay[1]; index++;
        texCoords[index] = verCoord[opt] + moveWay[2]; index++;
        texCoords[index] = verCoord[opt] + moveWay[0]; index++;
        texCoords[index] = verCoord[opt] + moveWay[2]; index++;
        texCoords[index] = verCoord[opt] + moveWay[3]; index++;
    }
}

void Cube::quad(int a, int b, int c, int d, int opt, int &index) {
    glm::vec4 n;
    for (int i = 0; i < 3; i++) {
        n[i] = vertices[a][i];
        n[i] += vertices[b][i];
        n[i] += vertices[c][i];
        n[i] += vertices[d][i];
    }
    n[3] = 0.0;
    glm::normalize(n);

    points[index] = vertices[a]; normals[index] = n; index++;
    points[index] = vertices[b]; normals[index] = n; index++;
    points[index] = vertices[c]; normals[index] = n; index++;
    points[index] = vertices[a]; normals[index] = n; index++;
    points[index] = vertices[c]; normals[index] = n; index++;
    points[index] = vertices[d]; normals[index] = n; index++;
    texturePos(opt, index - 6);
}

void Cube::makeQuad() {
    int index = 0;
    quad(1, 0, 3, 2, 2, index); // 왼쪽 팔
    quad(2, 3, 7, 6, 3, index); // 등
    quad(3, 0, 4, 7, 5, index); // 발
    quad(6, 5, 1, 2, 0, index); // 머리
    quad(6, 7, 4, 5, 4, index); // 오른쪽 팔
    quad(5, 4, 0, 1, 1, index); // 앞
}
