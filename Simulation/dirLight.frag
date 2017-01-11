#version 130

in vec3 vs_out_pos;
in vec3 vs_out_normal;
in vec2 vs_out_tex0;

out vec4 fs_out_col;

uniform vec3 eye_pos = vec3(0, 15, 15);

uniform int korong_up = 0;
uniform vec3 korong_pos;
uniform vec4 Ld2 = vec4(1.0f, 0.0f, 0.0f, 1.0f);

uniform vec3 lightdir = vec3( 1, -1, 1 );
uniform vec4 La = vec4(0.3f, 0.3f, 0.3f, 1);
uniform vec4 Ld = vec4(0.7f, 0.7f, 0.7f, 1);
uniform vec4 Ls = vec4(1, 0.6, 0.6, 1);

uniform vec4 Ka = vec4(0.3, 0.7, 0.4, 1);
uniform vec4 Kd = vec4(0.3, 0.7, 0.4, 1);
uniform vec4 Ks = vec4(0.3, 0.7, 0.4, 1);
uniform float specular_power = 16;
uniform sampler2D texImage;

void main()
{
	vec4 ambient = La * Ka;

	vec3 normal = normalize( vs_out_normal );
	vec3 toLight = normalize(-lightdir);
	float di = clamp( dot( toLight, normal), 0.0f, 1.0f );
	vec4 diffuse = Ld*Kd*di;

	vec4 diffuse2 = vec4(0);
	if(korong_up == 1){
		vec3 normal2 = normalize( vs_out_normal );
		vec3 toLight2 = normalize( korong_pos - vs_out_pos );
		float di2 = clamp( dot( toLight2, normal2), 0.0f, 1.0f );
		diffuse2 = Ld2*Kd*di2;
	}

	vec4 specular = vec4(0);
	if ( di > 0 )
	{
		vec3 e = normalize( eye_pos - vs_out_pos );
		vec3 r = reflect( -toLight, normal );
		float si = pow( clamp( dot(e, r), 0.0f, 1.0f ), specular_power );
		specular = Ls*Ks*si;
	}

	fs_out_col = (ambient + diffuse + diffuse2 + specular ) * texture(texImage, vs_out_tex0.st);
}
