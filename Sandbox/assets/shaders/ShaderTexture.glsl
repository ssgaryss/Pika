// test shader

#VERTEX_BEGIN()
#version 460 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
out vec2 v_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;
    
void main(){
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0f);
    v_TexCoord = a_TexCoord;
}
#VERTEX_END()


#FRAGMENT_BEGIN()
#version 460 core

layout (location = 0) out vec4 FragmentColor;
in vec2 v_TexCoord;

uniform sampler2D u_Texture0;

void main(){
    FragmentColor = texture(u_Texture0, v_TexCoord);
}
#FRAGMENT_END()
