struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float3 albedo    : COLOR;
};

struct PSInput
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float3 albedo : COLOR;
};

struct gbuffer
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float4 albedo : COLOR;
};

VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    output.worldPos = input.position;
    output.normal = normalize(input.normal);
    output.albedo = float3(1.f, 1.f, 1.f);
    return output;
}

gbuffer PSMain(PSInput input) : SV_TARGET
{
    gbuffer oGbuffer;
    oGbuffer.worldPos = input.worldPos;
    oGbuffer.normal = input.normal;
    oGbuffer.albedo = float4(input.albedo, 1.f);
    return oGbuffer;
}