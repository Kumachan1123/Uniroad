#include "Header.hlsli"

Texture2D DiffuseTexture : register(t0);

SamplerState Sampler : register(s0);

struct PS_Input
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

float4 main(PS_Input input) : SV_TARGET
{
    /// ディレクショナルライト
    float3 lightDirection =
        normalize(float3(-0.4f, -1.0f, -0.3f));

    /// 拡散反射
    float diffuse =
        saturate(dot(input.Normal, -lightDirection));

    /// 環境光
    diffuse =
        max(diffuse, 0.2f);

    float4 textureColor =
        DiffuseTexture.Sample(
            Sampler,
            input.TexCoord);

    return
        textureColor *
        DiffuseColor *
        diffuse;
}