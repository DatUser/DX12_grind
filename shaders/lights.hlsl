Texture2D<float4>	texAlbedo;
Texture2D<float3>	texNormals;
Texture2D<float3>	texPos;

RWTexture2D<float4>		texLights;

cbuffer LightData : register(b1)
{
    float3	worldpos;
    float3	albedo;
    float	fLuminance;
};


[numthreads(8, 8, 1)]
void CSMain(uint3 threadID : SV_DispatchThreadID)
{
}