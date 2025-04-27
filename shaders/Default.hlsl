struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 worldPos : POSITION;
    float4 normal : NORMAL;
    float4 position : SV_POSITION;
    float3 color    : COLOR;
};

struct GS_IN
{
    float4 worldPos : POSITION;
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float3 color : COLOR;
};

struct GS_OUT
{
    float4 worldPos : POSITION;
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float3 color : COLOR;
};

struct PSInput
{
    float4 worldPos : POSITION;
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
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
    output.normal = float4(normalize(input.normal), 1.);
    return output;
}

[maxvertexcount(2)]
void GS_Main(point GS_IN input[1], inout LineStream<GS_OUT> outputStream)
{
    GS_OUT output;

    // Start point
    output.worldPos = input[0].worldPos; // Adjust as needed
    output.position = input[0].position; // Adjust as needed
    output.normal = input[0].normal;
    output.color = input[0].color;
    outputStream.Append(output);

    // End point
    output.worldPos = input[0].worldPos + input[0].normal * 2.0;
    output.position = input[0].position; // Adjust as needed
    output.normal = input[0].normal;
    output.color = input[0].color;
    outputStream.Append(output);

    outputStream.RestartStrip();
}

float4 PSDefaultMain(PSInput input) : SV_TARGET
{
    return input.normal;//input.color;
    //return float4(1.0,1.0,1.0,1.0);
}