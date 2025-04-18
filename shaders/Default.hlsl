struct VertexInput
{
    float3 position : POSITION;
};

struct VertexOutput
{
    float4 worldPos : POSITION;
    float4 position : SV_POSITION;
    float3 color    : COLOR;
};

struct PSInput
{
    float4 worldPos : POSITION;
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

cbuffer FrameBuffer : register(b0)
{
    matrix mView;
    matrix mProj;
    matrix mViewProj;
};

matrix mModel;

VertexOutput VSDefaultMain(VertexInput input)
{
    VertexOutput output;
    output.worldPos = float4(input.position, 1.);
    output.position = mul(output.worldPos, mViewProj);
    return output;
}

float4 PSDefaultMain(PSInput input) : SV_TARGET
{
    return float4(1.f, 1.f, 1.f, 1.f);//input.color;
    //return float4(input.color, 1.0f);
}