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
	mat4 m_LightSpaceMatrix;
	vec3 m_Direction;
	vec3 m_LightColor;
	float m_Intensity;
	int m_ShawdowMapIndex;
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
	int m_ShawdowMapIndex;
	float m_LightSize;
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

uniform sampler2D u_Textures[27]; // Slots : 0, 1, 2, ..., 26
uniform sampler2D u_DirectionLightShadowMap; // Slots : 27
uniform samplerCube u_PointLightShadowMaps[4]; // Slots : 28, 29, 30, 31

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_Position;
in vec3 v_ViewPosition;
in flat highp int v_EntityID;

// Lights Calculation
vec3 calculateDirectionLights(DirectionLight vLight, vec3 vNormal, vec3 vViewPosition, vec3 vPosition);
vec3 calculatePointLights(PointLight vLight, vec3 vNormal, vec3 vViewPosition, vec3 vPosition);

// Shadow Calculation
float calculateDirectionLightShadow(DirectionLight vLight, vec3 vPosition);
float calculatePointLightShadow(PointLight vLight, vec3 vPosition);

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
		Result += calculateDirectionLights(u_DirectionLight[i], v_Normal, v_ViewPosition, v_Position);
	}
	// Point Lights
	for (int i = 0; i < MAX_NUM_OF_POINT_LIGHTS; ++i) {
		Result += calculatePointLights(u_PointLight[i], v_Normal, v_ViewPosition, v_Position);
	}
	o_FragmentColor = vec4(Result, 1.0);

	o_EntityID = v_EntityID;
}

vec3 calculateDirectionLights(DirectionLight vLight, vec3 vNormal, vec3 vViewPosition, vec3 vPosition) {
	if (vLight.m_Intensity == 0.0)
		return vec3(0.0);
	vec3 LightDir = normalize(-vLight.m_Direction);
	vec3 ViewDir = normalize(vViewPosition - vPosition);
	vec3 Normal = normalize(vNormal);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float Diff = max(dot(Normal, LightDir), 0.0);
	float Spec = pow(max(dot(Normal, HalfDir), 0.0), u_Shininess);
	vec3 Diffuse = vec3(1.0);
	vec3 Specular = vec3(1.0);
	float Shadow = 0.0;
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

	Shadow = calculateDirectionLightShadow(vLight, vPosition);

	return (Diffuse + Specular) * (1.0 - Shadow) * vLight.m_Intensity;
}

vec3 calculatePointLights(PointLight vLight, vec3 vNormal, vec3 vViewPosition, vec3 vPosition) {
	if (vLight.m_Intensity == 0.0)
		return vec3(0.0);
	vec3 LightDir = normalize(vLight.m_Position - v_Position);
	vec3 ViewDir = normalize(vViewPosition - vPosition);
	vec3 Normal = normalize(vNormal);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float Distance = length(vLight.m_Position - vPosition);
	float Attenuation = 1.0 / (vLight.m_Constant + vLight.m_Linear * Distance + vLight.m_Quadratic * (Distance * Distance));
	float Diff = max(dot(Normal, LightDir), 0.0);
	float Spec = pow(max(dot(Normal, HalfDir), 0.0), u_Shininess);
	vec3 Diffuse = vec3(1.0);
	vec3 Specular = vec3(1.0);
	float Shadow = 0.0;
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

	Shadow = calculatePointLightShadow(vLight, vPosition);

	return (Diffuse + Specular) * (1.0 - Shadow) * vLight.m_Intensity * Attenuation;
}

float calculateDirectionLightShadow(DirectionLight vLight, vec3 vPosition) {
	if (vLight.m_ShawdowMapIndex == -1)
		return 0.0;
	vec4 LightSpacePosition = vLight.m_LightSpaceMatrix * vec4(vPosition, 1.0);
	vec3 ProjectionCoords = LightSpacePosition.xyz / LightSpacePosition.w; // 从齐次坐标转换为欧拉坐标
	ProjectionCoords = ProjectionCoords * 0.5 + 0.5; // range[0, 1]
	if (ProjectionCoords.x >= 1.0 || ProjectionCoords.x <= 0.0 || ProjectionCoords.y >= 1.0 || ProjectionCoords.y <= 0.0)
		return 0.0;
	float ClosestDepth = texture(u_DirectionLightShadowMap, ProjectionCoords.xy).r;
	float CurrentDepth = ProjectionCoords.z;
	float Bias = 0.005; // 偏移量，可以根据场景调整大小
	float Shadow = (CurrentDepth - Bias) > ClosestDepth ? 1.0 : 0.0;
	return Shadow;
}

float calculatePointLightShadow(PointLight vLight, vec3 vPosition) {
	if (vLight.m_ShawdowMapIndex == -1)
		return 0.0;
	vec3 LightToPosition = vPosition - vLight.m_Position;
	float ClosestDepth = texture(u_PointLightShadowMaps[vLight.m_ShawdowMapIndex], LightToPosition).r;
	float CurrentDepth = length(LightToPosition / vLight.m_LightSize);
	float Bias = 0.005; // 偏移量，可以根据场景调整大小
	float Shadow = (CurrentDepth - Bias) > ClosestDepth ? 1.0 : 0.0;
	return Shadow;
}

#FRAGMENT_END()