struct gbuffer
{
    float3 worldPos : SV_TARGET0;
    float3 normal : SV_TARGET1;
    float4 albedo : SV_TARGET2;
};

struct LightData
{
}


[numthreads(8, 8, 1)]
void main_cs(uint3 threadID : SV_DispatchThreadID)
{
}