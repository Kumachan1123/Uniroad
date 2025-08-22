cbuffer ConstBuffer : register(b0)
{
    float4 windowSize;
    float2 frame;
    float animCount;
    float renderRatio;
    
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};