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

uniform matrix iMVPMat;
//float4x4 iMVPMat = {
//     2.41421356f, 0.0f,        0.0f,        0.0f ,
//     0.0f,        2.41421356f, 0.0f,        0.0f ,
//     0.0f,        0.0f,       -1.001001f,   1.0f ,
//     0.0f,        0.0f,       -0.1001001f,  0.0f
//};

VertexOutput VSDefaultMain(VertexInput input)
{
    VertexOutput output;
    output.position = mul(iMVPMat, float4(input.position, 1.));
    output.color = input.position;
    return output;
}

float4 PSDefaultMain(PSInput input) : SV_TARGET
{
    return float4(1.f, 1.f, 1.f, 1.f);//input.color;
    //return float4(input.color, 1.0f);
}