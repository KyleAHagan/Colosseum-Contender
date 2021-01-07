#version 430

//ex_Color will be interpolated between the vertices of the triangle to give a value to each fragment.
in vec4 ex_Color;
//texture coordinate will be interpolated between the vertices of the triangle to give a value to each fragment.
in vec2 ex_TextureCoordinates;

layout(location = 7) uniform sampler2D uTexture;

out vec4 out_Color;

void main()
{

	out_Color = vec4(1.0,0.0,0.0,1.0);

}
