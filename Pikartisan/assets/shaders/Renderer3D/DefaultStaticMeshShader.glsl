#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_EntityID;

layout(std140, binding = 0) uniform CameraData
{
	mat4 u_ViewProjectionMatrix;
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

out vec3 v_TexCoords;
out flat int v_EntityID;

void main() {
	v_TexCoords = a_Position;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0f);
}
#VERTEX_END()

#FRAGMENT_BEGIN()
#version 460 core
layout(location = 0) out vec4 o_FragmentColor;
layout(location = 1) out int o_EntityID;

in vec3 v_TexCoords;
in flat int v_EntityID;

void main() {
	o_FragmentColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	o_EntityID = v_EntityID;
}
#FRAGMENT_END()