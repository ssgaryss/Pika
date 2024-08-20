#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_TextureIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

out vec4 v_Color;
out vec2 v_TexCoord;
out flat int v_TextureIndex;
out float v_TilingFactor;
out flat int v_EntityID;

layout(std140, binding = 0) uniform CameraData
{
	mat4 u_ViewProjectionMatrix;
};

void main() {
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TextureIndex = a_TextureIndex;
	v_TilingFactor = a_TilingFactor;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0f);
}
#VERTEX_END()

#FRAGMENT_BEGIN()
#version 460 core

layout(location = 0) out vec4 o_FragmentColor;
layout(location = 1) out int o_EntityID;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat int v_TextureIndex;
in float v_TilingFactor;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];

void main() {
	o_EntityID = v_EntityID;
	vec4 Color = texture(u_Textures[v_TextureIndex], v_TexCoord * v_TilingFactor) * v_Color;
	float AlphaThreshold = 0.1;
	if (Color.a < AlphaThreshold) // 不要透明部分
		discard;
	o_FragmentColor = Color;
}
#FRAGMENT_END()