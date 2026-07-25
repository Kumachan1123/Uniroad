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
    float2 TexCoord : TEXCOORD0;
};


VS_Output main(VS_Input input)
{
    VS_Output output;


    output.Position =
        mul(
            float4(input.Position, 1),
            WorldViewProj);


    output.TexCoord =
        input.TexCoord;


    return output;
}