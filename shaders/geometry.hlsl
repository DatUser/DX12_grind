struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float3 albedo    : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float3 albedo : COLOR;
};

struct gbuffer
{
    float4 worldPos : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 albedo : SV_TARGET2;
};

cbuffer FrameBuffer : register(b0)
{
    matrix mView;
    matrix mProj;
    matrix mViewProj;
};

float4 WorldToScreenSpace(float4 pos)
{
    return mul(pos, mViewProj);
}

VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    output.worldPos = input.position;
    output.position = WorldToScreenSpace(float4(output.worldPos, 1.f));
    output.normal = normalize(input.normal);
    output.albedo = float3(1.f, 1.f, 1.f);
    return output;
}

gbuffer PSMain(PSInput input)
{
    gbuffer oGbuffer;
    oGbuffer.worldPos = float4(input.worldPos, 1.);
    oGbuffer.normal = float4(input.normal, -1.);
    oGbuffer.albedo = float4(input.albedo, 1.f);
    return oGbuffer;
}