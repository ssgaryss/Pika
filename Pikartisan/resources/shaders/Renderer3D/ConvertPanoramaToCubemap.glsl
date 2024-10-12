#VERTEX_BEGIN()
#version 460 core
precision highp float;

out vec2 v_TexCoord;

void main(void)
{
	float U = float((gl_VertexID & 1) << 2);
	float V = float((gl_VertexID & 2) << 1); // little trick where you don't need to upload vertex data.
	v_TexCoord.x = U * 0.5;
	v_TexCoord.y = V * 0.5;
	gl_Position = vec4(U - 1.0, V - 1.0, 0, 1);
}
#VERTEX_END()

#GEOMETRY_BEGIN()
#version 460 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

in vec2 v_TexCoord[];
out vec2 g_TexCoord;

vec3 convertUVToXYZ(int vFace, vec2 vUV); // 将纹理坐标映射到立方体的3D坐标
vec2 convertDirToUV(vec3 vDirection);     // 将立方体的方向向量转换为全景图像上的纹理坐标

void main() {
	for (int Face = 0; Face < 6; ++Face) {
		gl_Layer = Face;
		for (int i = 0; i < 3; ++i) {
			vec2 TexCoord = v_TexCoord[i] * 2.0 - 1.0; // 将 0,1 坐标映射到 -1,1 坐标
			vec3 Direction = normalize(convertUVToXYZ(Face, TexCoord));
			g_TexCoord = convertDirToUV(Direction);
			gl_Position = vec4(TexCoord, 0.0, 1.0);
			EmitVertex();
		}
		EndPrimitive();
	}
}

vec3 convertUVToXYZ(int vFace, vec2 vUV) {
	if (vFace == 0)
		return vec3(1.0f, vUV.y, -vUV.x);
	else if (vFace == 1)
		return vec3(-1.0f, vUV.y, vUV.x);
	else if (vFace == 2)
		return vec3(+vUV.x, -1.0f, +vUV.y);
	else if (vFace == 3)
		return vec3(+vUV.x, 1.0f, -vUV.y);
	else if (vFace == 4)
		return vec3(+vUV.x, vUV.y, 1.0f);
	else if (vFace == 5)
		return vec3(-vUV.x, +vUV.y, -1.0f);
}

vec2 convertDirToUV(vec3 vDirection) {
	return vec2(0.5f + 0.5f * atan(vDirection.z, vDirection.x) / 3.14159265359, 1.0f - acos(vDirection.y) / 3.14159265359);
}

#GEOMETRY_END()

#FRAGMENT_BEGIN()
#version 330 core

in vec2 g_TexCoord;    // 从顶点着色器传递的纹理坐标
out vec4 o_FragColor;  // 最终输出的颜色

uniform sampler2D u_Panorama;  // 输入的全景图像纹理

void main()
{
	vec3 Color = texture(u_Panorama, g_TexCoord).rgb;
	o_FragColor = vec4(Color, 1.0);  // 输出最终颜色
}
#FRAGMENT_END()