#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

out vec4 fvertex;
out vec3 fnormal;
out vec3 fmatamb;
out vec3 fmatdiff;
out vec3 fmatspec;
out float fmatshin;

// Valors per als components que necessitem del focus de llum

void main()
{	
	fvertex = view*TG*vec4(vertex, 1.0);
	fnormal = inverse(transpose(mat3(view*TG)))*normal;
	fmatamb = matamb;
	fmatdiff = matdiff;
	fmatspec = matspec;
	fmatshin = matshin;

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
