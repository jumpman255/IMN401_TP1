#version 410

uniform mat4 gProjectionMatrix;
uniform mat4 gViewMatrix;
uniform mat4 gModelMatrix;

uniform vec4 uColor;

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

struct FS_In {
    vec3 Color;
	vec2 TexCoord;
	vec3 Normal;
	vec3 WorldPosition;
};

out FS_In fsIn;

void main()
{
	fsIn.WorldPosition = (gModelMatrix * vec4(aPosition, 1.0f)).xyz;
	gl_Position = gProjectionMatrix * gViewMatrix * vec4(fsIn.WorldPosition, 1.0f);
	fsIn.TexCoord = aTexCoord;
	fsIn.Color = uColor.rgb;
	fsIn.Normal = mat3(transpose(inverse(gModelMatrix))) * aNormal;
}
