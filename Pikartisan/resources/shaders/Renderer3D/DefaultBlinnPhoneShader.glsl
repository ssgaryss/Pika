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

struct PointLightData {
	vec3 m_Position;
	vec3 m_LightColor;
	float m_Intensity;
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};

layout(std140, binding = 2) uniform PointLightsData
{
	PointLightData u_PointLight[4];
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

const vec3 lightPos = vec3(0.0, 10.0, 0.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const vec3 objectColor = vec3(1.0, 0.0, 1.0);

void main() {
	// ambient
	float AmbientStrength = 0.1;
	vec3 Ambient = AmbientStrength * lightColor;

	// diffuse 
	vec3 Normal = normalize(v_Normal);
	vec3 LightDir = normalize(lightPos - v_Position);
	float diff = max(dot(Normal, LightDir), 0.0);
	vec3 Diffuse = diff * lightColor;

	// specular
	vec3 ViewDir = normalize(v_ViewPosition - v_Position);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float spec = pow(max(dot(Normal, HalfDir), 0.0), 8.0);
	vec3 Specular = spec * lightColor;

	vec3 Result = (Ambient + Diffuse + Specular) * objectColor;
	o_FragmentColor = vec4(Result, 1.0);

	o_EntityID = v_EntityID;
}
#FRAGMENT_END()