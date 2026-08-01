#include "Header.hlsli"

struct VS_Input
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct VS_Output
{
    float4 Position : SV_POSITION;
    float3 WorldPosition : TEXCOORD1;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

VS_Output main(VS_Input input)
{
    VS_Output output;

    float4 worldPosition =
        mul(
            float4(input.Position, 1.0f),
            World);

    output.WorldPosition =
        worldPosition.xyz;

    output.Position =
        mul(
            worldPosition,
            WorldViewProj);

    output.Normal =
        normalize(
            mul(
                float4(input.Normal, 0.0f),
                WorldInverseTranspose).xyz);

    output.TexCoord =
        input.TexCoord;

    return output;
}