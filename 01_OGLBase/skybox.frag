#version 330 core

in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;
uniform vec3 La;

void main()
{
	// Ambiens
	vec3 ambient = La;
	fs_out_col = vec4(ambient, 1) * texture( skyboxTexture, (vs_out_pos));
}
