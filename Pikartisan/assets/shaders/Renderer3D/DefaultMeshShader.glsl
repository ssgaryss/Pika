#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Position;

layout(std140, binding = 1) uniform CameraData
{
	mat4 u_ViewProjectionMatrix;
};

out vec3 v_TexCoords;

void main() {
	v_TexCoords = a_Position;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0f);
}
#VERTEX_END()

#FRAGMENT_BEGIN()
#version 460 core
layout(location = 0) out vec4 o_FragmentColor;
layout(location = 1) out int o_EntityID;

in vec3 v_TexCoords;

uniform samplerCube u_EnvironmentMap;

void main() {
	vec3 EnvironmentLight = texture(u_EnvironmentMap, v_TexCoords).rgb;
	o_FragmentColor = vec4(EnvironmentLight, 1.0f);
	o_EntityID = -1;
}
#FRAGMENT_END()