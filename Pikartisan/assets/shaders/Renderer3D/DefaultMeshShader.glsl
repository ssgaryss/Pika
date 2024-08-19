#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

layout(std140, binding = 1) uniform CameraData
{
    mat4 u_ViewProjectionMatrix;
};

out vec2 v_TexCoords;

void main() {
    v_TexCoords = a_TexCoords;
    gl_Position = vec4(a_Pos, 1.0);
}
#VERTEX_END()

#FRAGMENT_BEGIN()
#version 460 core
layout(location = 0) out vec4 o_FragmentColor;

in vec2 v_TexCoords;

uniform sampler2D u_EnvironmentMap;

void main() {
    vec3 EnvironmentLight = texture(u_EnvironmentMap, v_TexCoords).rgb;
    o_FragmentColor = vec4(EnvironmentLight, 1.0);
}
#FRAGMENT_END()