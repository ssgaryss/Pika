#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Position;

uniform mat4 u_LightSpaceMatrix;

void main() {
	gl_Position = u_LightSpaceMatrix * vec4(a_Position, 1.0f);
}
#VERTEX_END()

#FRAGMENT_BEGIN()
#version 460 core

void main() {
	//gl_FragDepth = gl_FragCoord.z;
}
#FRAGMENT_END()