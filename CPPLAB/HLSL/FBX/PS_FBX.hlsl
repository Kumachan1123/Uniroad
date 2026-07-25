#include "Header.hlsli"
Texture2D DiffuseTexture : register(t0);

SamplerState Sampler : register(s0);


struct PS_Input
{
    float4 Position : SV_POSITION;

    float2 TexCoord : TEXCOORD0;
};


float4 main(PS_Input input) : SV_TARGET
{
    //return float4(1, 0, 1, 1);
    return DiffuseTexture.Sample(Sampler, input.TexCoord) * DiffuseColor;
}