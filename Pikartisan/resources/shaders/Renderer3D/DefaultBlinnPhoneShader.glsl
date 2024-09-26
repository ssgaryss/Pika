#VERTEX_BEGIN()
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in highp int a_EntityID;

layout(std140, binding = 0) uniform CameraData
{
	mat4 u_ViewProjectionMatrix;
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

out vec3 v_Normal;
out vec3 v_Position;
out vec3 v_ViewPosition;
out flat highp int v_EntityID;

void main() {
	v_Normal = a_Normal;
	v_Position = a_Position;
	v_ViewPosition = inverse(u_ViewMatrix)[3].xyz;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0f);
}
#VERTEX_END()

#FRAGMENT_BEGIN()
#version 460 core
layout(location = 0) out vec4 o_FragmentColor;
layout(location = 1) out highp int o_EntityID;

#define MAX_NUM_OF_POINT_LIGHTS 1   // for now
struct PointLight {
	vec3 m_Position;
	vec3 m_LightColor;
	float m_Intensity;
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};

layout(std140, binding = 2) uniform PointLights
{
	PointLight u_PointLight[MAX_NUM_OF_POINT_LIGHTS];
};

layout(std140, binding = 4) uniform BlinnPhoneMaterial
{
	vec3 u_Ambient;
	vec3 u_Diffuse;
	vec3 u_Specular;
};

in vec3 v_Normal;
in vec3 v_Position;
in vec3 v_ViewPosition;
in flat highp int v_EntityID;

vec3 calculatePointLights(PointLight vLight, vec3 vNormal, vec3 vPosition, vec3 vViewDir);

void main() {
	vec3 Result = vec3(0.0);
	for (int i = 0; i < MAX_NUM_OF_POINT_LIGHTS; ++i) {
		Result += calculatePointLights(u_PointLight[i], v_Normal, v_Position, v_ViewPosition - v_Position);
	}
	vec3 N = normalize(v_Normal);
	o_FragmentColor = vec4(N, 1.0);

	o_EntityID = v_EntityID;
}

vec3 calculatePointLights(PointLight vLight, vec3 vNormal, vec3 vPosition, vec3 vViewDir) {
	vec3 LightDir = normalize(vLight.m_Position - v_Position);
	vec3 ViewDir = normalize(vViewDir);
	vec3 Normal = normalize(vNormal);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float diff = max(dot(vNormal, LightDir), 0.0);
	float spec = pow(max(dot(Normal, HalfDir), 0.0), 8.0);
	float AmbientStrength = 0.1;
	vec3 Diffuse = diff * vLight.m_LightColor * u_Diffuse;
	vec3 Specular = spec * vLight.m_LightColor * u_Specular;
	vec3 Ambient = AmbientStrength * vLight.m_LightColor * u_Ambient;
	return (Diffuse + Specular + Ambient);
}

#FRAGMENT_END()