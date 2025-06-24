#include "brdf.hlsl"

Texture2D<float4>	texAlbedo		: register(t0);
Texture2D<float3>	texNormals		: register(t1);
Texture2D<float3>	texPos		: register(t2);

RWTexture2D<float4>		texLights : register(u0);

cbuffer LightData : register(b1)
{
    float3	worldpos;
    float3	albedo;
    float	fLuminance;
	float	fHereForAlignment;
};


[numthreads(8, 8, 1)]
void CSMain(uint3 threadID : SV_DispatchThreadID)
{
	//float4 albedo = texAlbedo[threadID.xy];
	if (texAlbedo[threadID.xy].a == 0.f)
	{
		texLights[threadID.xy] = texLights[threadID.xy];
		return;
	}

	//float3 camPos = float3(0., 0., -10.);// temp observer pos
	//float3 wo = normalize(camPos - texPos[threadID.xy]);
	//float3 wi = normalize(worldpos - texPos[threadID.xy]);
	//float3 n = normalize(texNormals[threadID.xy]);
	//float3 albedo = 1.;

	float3 finalColor = 1.;//cook_torrance_brdf(wo, wi, n, albedo);
	texLights[threadID.xy] = float4(finalColor, 1.);
}