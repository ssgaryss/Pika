#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Position;

void main() {
	gl_Position = vec4(a_Position, 1.0f);
}
#VERTEX_END()

#GEOMETRY_BEGIN()
#version 460 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

out vec4 g_Position;

uniform mat4 u_LightSpaceMatrices[6];

void main() {
	for (int Face = 0; Face < 6; ++Face) {
		gl_Layer = Face;
		for (int i = 0; i < 3; ++i) {
			g_Position = gl_in[i].gl_Position;
			gl_Position = u_LightSpaceMatrices[Face] * g_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
}
#GEOMETRY_END()

#FRAGMENT_BEGIN()
#version 460 core

in vec4 g_Position;

uniform vec3 u_PointLightPosition;
uniform float u_LightSize;

void main() {
	float LightDistance = length(g_Position.xyz - u_PointLightPosition);

	// map to [0;1] range by dividing by far_plane
	LightDistance = LightDistance / u_LightSize; // ÔÝÊ±Îª25.0

	// write this as modified depth
	gl_FragDepth = LightDistance;
}
#FRAGMENT_END()