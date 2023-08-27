#version 330 core

// VBO-ból érkező változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;
out vec3 vs_out_eye_pos;
out vec3 vs_out_light_dir;
out vec3 vs_out_la;
out vec3 vs_out_ls;

// shader külső paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec3 ambientCol;
uniform vec3 sunLight;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	
	vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
	vs_out_eye_pos = lightPos;
	vs_out_light_dir = lightDir;
	vs_out_la = ambientCol;
	vs_out_ls = sunLight;
}