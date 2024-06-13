#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in float a_TilingFactor;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TextureIndex;
out float v_TilingFactor;

uniform mat4 u_ViewProjectionMatrix;

void main() {
	v_Color = a_Color;
	v_TextureIndex = a_TextureIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0f);
	v_TexCoord = a_TexCoord;
}
#VERTEX_END()

#FRAGMENT_BEGIN()
#version 460 core

layout(location = 0) out vec4 FragmentColor;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TextureIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main() {
	FragmentColor = texture(u_Textures[int(v_TextureIndex)], v_TexCoord * v_TilingFactor) * v_Color;
}
#FRAGMENT_END()