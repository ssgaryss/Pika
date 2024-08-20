#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_EntityID;

out vec4 v_Color;
out flat int v_EntityID;

layout(std140, binding = 0) uniform CameraData
{
	mat4 u_ViewProjectionMatrix;
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

void main() {
	v_Color = a_Color;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0f);
}
#VERTEX_END()

#FRAGMENT_BEGIN()
#version 460 core

layout(location = 0) out vec4 o_FragmentColor;
layout(location = 1) out int o_EntityID;

in vec4 v_Color;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];

void main() {
	o_FragmentColor = v_Color;
	o_EntityID = v_EntityID;
}
#FRAGMENT_END()