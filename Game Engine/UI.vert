#version 430

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in vec2 in_TextureCoordinates;
layout(location = 3) uniform mat4 translationRotationScale; 

out vec4 ex_Color;
out vec2 ex_TextureCoordinates;

void main()
{

	gl_Position = translationRotationScale * vec4(in_Position);

	//ex_Color will be interpolated between the vertices of the triangle to give a value to each fragment.
	ex_Color = in_Color;

	//texture coordinate will be interpolated between the vertices of the triangle to give a value to each fragment.
	ex_TextureCoordinates = in_TextureCoordinates;

}