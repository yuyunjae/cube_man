#version 430

const int NO_LIGHT = 0, GOURAUD = 1, PHONG = 2;

in vec4 fragPos;
in vec4 color;
in vec4 normal;
in vec2 texCoord;

out vec4  fColor;

uniform int shadeMode;
uniform mat4 mView;
uniform sampler2D cubeTextures;

void main() 
{ 
	vec4 L = normalize(vec4(4, 3, 5, 0));
	float kd = 0.8, ks = 1.0, ka = 0.2, shininess = 60;
	vec4 Id = color;
	vec4 Is = vec4(1, 1, 1, 1);
	vec4 Ia = color;

	if (shadeMode == NO_LIGHT)
		fColor = texture( cubeTextures, texCoord ).rgba;
	else if (shadeMode == GOURAUD)
		fColor = color * texture( cubeTextures, texCoord ).rgba;
	else if (shadeMode == PHONG)
	{
		// ambient
		float ambient = ka;

		// diffuse
		vec4 N = normalize(normal);
		float diff = kd * clamp(dot(N, L), 0, 1);

		// specular
		vec4 viewPos = inverse(mView) * vec4(0, 0, 0, 1);
		vec4 V =  normalize(viewPos - fragPos);
		vec4 R = reflect(-L, N);
		float spec = ks * pow(clamp(dot(V, R), 0, 1), shininess);

		fColor = ambient * Ia + diff * Id + spec * Is;
		fColor = fColor * texture( cubeTextures, texCoord ).rgba;
	}
} 

