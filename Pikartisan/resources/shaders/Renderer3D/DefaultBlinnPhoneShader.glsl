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
out vec2 v_TexCoord;
out vec3 v_Position;
out vec3 v_ViewPosition;
out flat highp int v_EntityID;

void main() {
	v_Normal = a_Normal;
	v_TexCoord = a_TexCoord;
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

#define MAX_NUM_OF_DIRECTION_LIGHTS 1
struct DirectionLight {
	vec3 m_Direction;
	vec3 m_LightColor;
	uint u_ShawdowMapIndex;
	float m_Intensity;
};
layout(std140, binding = 1) uniform DirectionLights
{
	DirectionLight u_DirectionLight[MAX_NUM_OF_DIRECTION_LIGHTS];
};

#define MAX_NUM_OF_POINT_LIGHTS 4
struct PointLight {
	vec3 m_Position;
	vec3 m_LightColor;
	float m_Intensity;
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
	uint u_ShawdowMapIndex;
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
	float u_Shininess;
	uint u_DiffuseMapSlot;
	uint u_SpecularMapSlot;
};

uniform sampler2D u_Textures[32];

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_Position;
in vec3 v_ViewPosition;
in flat highp int v_EntityID;

vec3 calculateDirectionLights(DirectionLight vLight, vec3 vNormal, vec3 vViewDir);
vec3 calculatePointLights(PointLight vLight, vec3 vNormal, vec3 vPosition, vec3 vViewDir);

void main() {
	vec3 Result = vec3(0.0);
	// Ambient
	float AmbientStrength = 0.1;
	if (u_DiffuseMapSlot == 0) {
		Result += AmbientStrength * u_Ambient;
	}
	else {
		Result += AmbientStrength * texture(u_Textures[u_DiffuseMapSlot], v_TexCoord).rgb;
	}
	// Direction Lights
	for (int i = 0; i < MAX_NUM_OF_DIRECTION_LIGHTS; ++i) {
		Result += calculateDirectionLights(u_DirectionLight[i], v_Normal, v_ViewPosition - v_Position);
	}
	// Point Lights
	for (int i = 0; i < MAX_NUM_OF_POINT_LIGHTS; ++i) {
		Result += calculatePointLights(u_PointLight[i], v_Normal, v_Position, v_ViewPosition - v_Position);
	}
	o_FragmentColor = vec4(Result, 1.0);

	o_EntityID = v_EntityID;
}

vec3 calculateDirectionLights(DirectionLight vLight, vec3 vNormal, vec3 vViewDir) {
	vec3 LightDir = normalize(-vLight.m_Direction);
	vec3 ViewDir = normalize(vViewDir);
	vec3 Normal = normalize(vNormal);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float Diff = max(dot(Normal, LightDir), 0.0);
	float Spec = pow(max(dot(Normal, HalfDir), 0.0), u_Shininess);
	vec3 Diffuse = vec3(1.0);
	vec3 Specular = vec3(1.0);
	if (u_DiffuseMapSlot == 0) {
		Diffuse = Diff * vLight.m_LightColor * u_Diffuse;
	}
	else {
		Diffuse = Diff * vLight.m_LightColor * texture(u_Textures[u_DiffuseMapSlot], v_TexCoord).rgb;
	}

	if (u_SpecularMapSlot == 0) {
		Specular = Spec * vLight.m_LightColor * u_Specular;
	}
	else {
		Specular = Spec * vLight.m_LightColor * texture(u_Textures[u_SpecularMapSlot], v_TexCoord).rgb;
	}
	return (Diffuse + Specular) * vLight.m_Intensity;
}

vec3 calculatePointLights(PointLight vLight, vec3 vNormal, vec3 vPosition, vec3 vViewDir) {
	vec3 LightDir = normalize(vLight.m_Position - v_Position);
	vec3 ViewDir = normalize(vViewDir);
	vec3 Normal = normalize(vNormal);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float Distance = length(vLight.m_Position - vPosition);
	float Attenuation = 1.0 / (vLight.m_Constant + vLight.m_Linear * Distance + vLight.m_Quadratic * (Distance * Distance));
	float Diff = max(dot(Normal, LightDir), 0.0);
	float Spec = pow(max(dot(Normal, HalfDir), 0.0), u_Shininess);
	vec3 Diffuse = vec3(1.0);
	vec3 Specular = vec3(1.0);
	if (u_DiffuseMapSlot == 0) {
		Diffuse = Diff * vLight.m_LightColor * u_Diffuse;
	}
	else {
		Diffuse = Diff * vLight.m_LightColor * texture(u_Textures[u_DiffuseMapSlot], v_TexCoord).rgb;
	}

	if (u_SpecularMapSlot == 0) {
		Specular = Spec * vLight.m_LightColor * u_Specular;
	}
	else {
		Specular = Spec * vLight.m_LightColor * texture(u_Textures[u_SpecularMapSlot], v_TexCoord).rgb;
	}
	return (Diffuse + Specular) * vLight.m_Intensity * Attenuation;
}

#FRAGMENT_END()