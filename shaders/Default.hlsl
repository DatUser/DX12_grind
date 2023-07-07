struct VertexInput
{
    float3 position : POSITION;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 color    : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

VertexOutput VSDefaultMain(VertexInput input)
{
    VertexOutput output;
    output.position = float4(input.position, 1.);
    output.color = input.position;
    return output;
}

float4 PSDefaultMain(PSInput input) : SV_TARGET
{
    //return float4(1.f, 1.f, 1.f, 1.f);//input.color;
    return float4(input.color, 1.0f);
}