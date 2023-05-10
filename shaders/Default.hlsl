struct VertexInput
{
    float3 position : POSITION;
};

struct VertexOutput
{
    float3 position : SV_POSITION;
};

struct PSInput
{
    float4 color : COLOR;
};

VertexOutput VSDefaultMain(VertexInput input)
{
    VertexOutput output;
    output.position = input.position;
    return output;
}

float4 PSDefaultMain(PSInput input) : SV_TARGET
{
    return input.color;
}