struct VertexInput
{
    float4 position : POSITION;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
};

VertexOutput VSDefaultMain(VertexInput input)
{
    VertexOutput output;
    output.position = input.position;
    return output;
}