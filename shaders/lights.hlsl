
Texture2D<float4>	texAlbedo		: register(t0);
Texture2D<float4>	texPos		: register(t1);
Texture2D<float4>	texNormals		: register(t2);

RWTexture2D<float4>		texLights : register(u0);

struct Light
{
	float3	worldpos;
	float	fLuminance;
	float4	color;
};

cbuffer LightData : register(b1)
{
	Light light;
    //float3	worldpos;
    //float4	lightColor;
    //float	fLuminance;
	//float	fHereForAlignment;
};

#include "brdf.hlsl"

[numthreads(8, 8, 1)]
void CSMain(uint3 threadID : SV_DispatchThreadID)
{
	//float4 albedo = texAlbedo[threadID.xy];
	if (texAlbedo[threadID.xy].a == 0.f)
	{
		texLights[threadID.xy] = texLights[threadID.xy];
		return;
	}

	float3 camPos = float3(0., 0., -10.);// temp observer pos
	float3 wo = normalize(camPos - texPos[threadID.xy]);
	float3 wi = normalize(light.worldpos - texPos[threadID.xy]);
	//float3 n = normalize(texNormals[threadID.xy]);
	float3 n = normalize(texPos[threadID.xy]);//true for sphere
	float3 albedo = texAlbedo[threadID.xy];

	float3 finalColor = cook_torrance_brdf(wo, wi, n, albedo);
	texLights[threadID.xy] = float4(finalColor, 1.);
	//texLights[threadID.xy] = float4(albedo, 1.);
	//texLights[threadID.xy] = albedo;
}