#VERTEX_BEGIN()
#version 460 core
precision highp float;

out vec2 v_TexCoord;

void main(void)
{
    float x = float((gl_VertexID & 1) << 2);
    float y = float((gl_VertexID & 2) << 1); // little trick where you don't need to upload vertex data.
    v_TexCoord.x = x * 0.5;
    v_TexCoord.y = y * 0.5;
    gl_Position = vec4(x - 1.0, y - 1.0, 0, 1);
}
#VERTEX_END()

#FRAGMENT_BEGIN()
#version 330 core

in vec2 v_TexCoord;    // 从顶点着色器传递的纹理坐标
out vec4 o_FragColor;  // 最终输出的颜色

uniform sampler2D u_Panorama;  // 输入的全景图像纹理
uniform int u_CurrentFace;     // 当前渲染的立方体面索引

// 将纹理坐标映射到立方体的3D坐标
vec3 uvToXYZ(int face, vec2 uv)
{
    if (face == 0)
        return vec3(1.f, uv.y, -uv.x);

    else if (face == 1)
        return vec3(-1.f, uv.y, uv.x);

    else if (face == 2)
        return vec3(+uv.x, -1.f, +uv.y);

    else if (face == 3)
        return vec3(+uv.x, 1.f, -uv.y);

    else if (face == 4)
        return vec3(+uv.x, uv.y, 1.f);

    else //if(face == 5)
        return vec3(-uv.x, +uv.y, -1.f);
}

// 将立方体的方向向量转换为全景图像上的纹理坐标
vec2 dirToUV(vec3 dir)
{
    return vec2(
        0.5f + 0.5f * atan(dir.z, dir.x) / 3.14159265359,
        1.f - acos(dir.y) / 3.14159265359);
}

// 将全景图像的纹理坐标转换为立方体贴图上的颜色值
vec3 panoramaToCubeMap(int face, vec2 texCoord)
{
    vec2 texCoordNew = v_TexCoord * 2.0 - 1.0; // 将 0,1 坐标映射到 -1,1 坐标
    vec3 scan = uvToXYZ(face, texCoordNew);    // 获取当前面的 3D 坐标
    vec3 direction = normalize(scan);          // 归一化方向向量
    vec2 src = dirToUV(direction);             // 获取在全景图上的纹理坐标

    return texture(u_Panorama, src).rgb;       // 从全景图中采样颜色
}

void main()
{
    vec3 Color = panoramaToCubeMap(u_CurrentFace, v_TexCoord);
    o_FragColor = vec4(Color, 1.0);  // 输出最终颜色
}
#FRAGMENT_END()