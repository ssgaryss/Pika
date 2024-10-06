#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Position;

void main() {
	gl_Position = vec4(a_Position, 1.0f);
}
#VERTEX_END()

#GEOMETRY_BEGIN()
#version 460 core
layout(triangles) in

void main() {

}
#GEOMETRY_END()

#FRAGMENT_BEGIN()
#version 460 core

void main() {
	//gl_FragDepth = gl_FragCoord.z;
}
#FRAGMENT_END()