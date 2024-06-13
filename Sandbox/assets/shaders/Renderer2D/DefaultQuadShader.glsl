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
uniform mat4 u_Transform;

void main() {
	v_Color = a_Color;
	v_TextureIndex = a_TextureIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0f);
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

uniform vec4 u_TintColor;
uniform sampler2D u_Texture;

void main() {
	//FragmentColor = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_TintColor;
	FragmentColor = v_Color;
}
#FRAGMENT_END()