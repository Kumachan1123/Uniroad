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
    /// 仮の影色
    const float3 ShadowColor = float3(0.72f, 0.78f, 0.95f);

    /// 仮の環境光
    const float Ambient = 0.25f;

    /// ライト方向
    float3 lightDirection =
        normalize(LightDirection);

    /// ライト色
    float3 lightColor =
        LightColor;

    /// Half Lambert
    float NdotL =
        dot(
            normalize(input.Normal),
            -lightDirection);

    NdotL =
        NdotL * 0.5f + 0.5f;

    /// 3段階トゥーン
    float diffuse =
        Ambient;

    diffuse +=
        step(0.40f, NdotL) * 0.30f;

    diffuse +=
        step(0.75f, NdotL) * 0.45f;

    diffuse =
        saturate(diffuse);

    /// ライト色
    float3 toonLight =
        lerp(
            ShadowColor,
            float3(1.0f, 1.0f, 1.0f),
            diffuse);

    /// テクスチャ
    float4 textureColor =
        DiffuseTexture.Sample(
            Sampler,
            input.TexCoord);

    /// RGB
    float3 finalColor =
        textureColor.rgb *
        DiffuseColor.rgb *
        lightColor *
        toonLight;

    /// Alpha
    float finalAlpha =
        textureColor.a *
        DiffuseColor.a;
    //return float4(1, 0, 0, DiffuseColor.a);
    return float4(
        finalColor,
      0.25);
}