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
	//if (texAlbedo[threadID.xy].a == 0.f)
	//{
	//	texLights[threadID.xy] = float4(albedo.xyz, 1.f);
	//	return;
	//}

	//texLights[threadID.xy] = float4(1.f, 1.f, 1.f, 1.f);
	texLights[threadID.xy] = texAlbedo[threadID.xy];
}