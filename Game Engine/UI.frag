#version 430

//ex_Color will be interpolated between the vertices of the triangle to give a value to each fragment.
in vec4 ex_Color;
//texture coordinate will be interpolated between the vertices of the triangle to give a value to each fragment.
in vec2 ex_TextureCoordinates;

layout(location = 6) uniform vec2 textureCoordinateOffset;
layout(location = 7) uniform sampler2D uTexture;

out vec4 out_Color;

void main()
{

	out_Color = texture2D(uTexture, ex_TextureCoordinates + textureCoordinateOffset);

	if (out_Color.w < 0.01)
		discard;

}
