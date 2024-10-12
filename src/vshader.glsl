#version 150

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform mat4 mPVM;

void main() 
{
  gl_Position = mPVM * vPosition;
  color = vColor;
} 
