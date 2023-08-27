#version 330 core

// vertex shaderbol jovo ertekek
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimenő érték - a fragment színe
out vec4 fs_out_col;

// fény iránya:
uniform vec3 light_dir = vec3(0, -3, 0);

// fénytulajdonságok: ambiens, diffúz, skepularis
uniform vec3 Ls = vec3(0.1, 0.1, 0.1);
uniform vec3 La;
uniform vec3 Ld;
uniform sampler2D texImage;

void main()
{

	// Ambiens
	vec3 ambient = La;

	// Diffuz
	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	float cosa = clamp(dot(normal, to_light), 0, 1);
	vec3 diffuse = cosa * Ld;

	
	fs_out_col = vec4(ambient + diffuse + Ls, 1) * texture(texImage, vs_out_tex);
}
