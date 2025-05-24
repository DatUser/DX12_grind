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

float4 WorldToScreenSpace(float4 pos)
{
    return mul(pos, mViewProj);
}

VertexOutput VSDefaultMain(VertexInput input)
{
    VertexOutput output;
    output.worldPos = float4(input.position, 1.);
    output.position = WorldToScreenSpace(output.worldPos);
    output.normal = float4(normalize(input.normal), 1.);
    output.color = float3(1.f, 1.f, 1.f);
    return output;
}

//[maxvertexcount(2)]
//void GS_Main(point GS_IN input[1], inout LineStream<GS_OUT> outputStream)
[maxvertexcount(6)]
void GS_Main(triangle /*GS_IN*/VertexOutput input[3], inout LineStream<GS_OUT> outputStream)
{
    for (int i = 0; i < 3; i++)
    {
        GS_OUT output;

        // Start point
        output.worldPos = input[i].worldPos; // Adjust as needed
        output.position = input[i].position; // Adjust as needed
        output.normal = input[i].normal;
        output.color = input[i].color;
        outputStream.Append(output);

        // End point
        output.worldPos = input[i].worldPos + input[i].normal * 2.0;
        output.worldPos = 1.f;

        output.position = WorldToScreenSpace(output.worldPos);
        output.normal = input[i].normal;
        output.color = input[i].color;
        outputStream.Append(output);

        outputStream.RestartStrip();
    }

}

float4 PSDefaultMain(PSInput input) : SV_TARGET
{
    return input.normal;//input.color;
    //return float4(1.0,1.0,1.0,1.0);
}