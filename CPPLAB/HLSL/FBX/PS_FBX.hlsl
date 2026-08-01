#include "Header.hlsli"

Texture2D DiffuseTexture : register(t0);
Texture2D RampTexture : register(t1);

SamplerState Sampler : register(s0);
SamplerState RampSampler : register(s1);

struct PS_Input
{
    float4 Position : SV_POSITION;
    float3 WorldPosition : TEXCOORD1;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

float4 main(PS_Input input) : SV_TARGET
{
    // ブルアカ風設定
    const float Ambient = 0.18f;

    // ライトラップ
    const float Wrap = 0.25f;

    // リムライト
    const float3 RimColor = float3(0.75f, 0.88f, 1.0f);
    const float RimPower = 4.0f;
    const float RimIntensity = 0.35f;

    // 影境界色
    const float3 BoundaryColor = float3(0.82f, 0.90f, 1.0f);
    const float BoundaryIntensity = 0.18f;

    float3 normal =
        normalize(input.Normal);

    float3 lightDirection =
        normalize(LightDirection);

    float3 viewDirection =
        normalize(CameraPosition - input.WorldPosition);

    // ラップ付きHalf Lambert
    float NdotL =
        dot(normal, -lightDirection);

    NdotL =
        (NdotL + Wrap) / (1.0f + Wrap);

    NdotL =
        saturate(NdotL);

    // Ramp
    float ramp =
        RampTexture.Sample(
            RampSampler,
            float2(NdotL, 0.5f)).r;

    ramp =
        Ambient +
        ramp * (1.0f - Ambient);

    float4 textureColor =
        DiffuseTexture.Sample(
            Sampler,
            input.TexCoord);

    float3 baseColor =
        textureColor.rgb *
        DiffuseColor.rgb;

    // 基本ライティング
    float3 finalColor =
        baseColor *
        LightColor *
        ramp;

    // --------------------
    // 境界色
    // --------------------
    float boundary =
        smoothstep(0.45f, 0.55f, NdotL) -
        smoothstep(0.55f, 0.65f, NdotL);

    finalColor +=
        BoundaryColor *
        boundary *
        BoundaryIntensity;

    // --------------------
    // リムライト
    // --------------------
    float rim =
        pow(
            saturate(1.0f - dot(normal, viewDirection)),
            RimPower);

    // 明るい面では弱く
    rim *= (1.0f - ramp);

    // 輪郭だけ
    rim *= smoothstep(0.2f, 0.8f, rim);

    finalColor +=
        RimColor *
        rim *
        RimIntensity;
        //================================
    // スペキュラハイライト
    //================================

    const float3 SpecularColor = float3(1.0f, 1.0f, 1.0f);

    const float SpecularPower = 64.0f;
    const float SpecularIntensity = 0.15f;


    float3 halfVector =
        normalize(
            -lightDirection +
            viewDirection);


    float specular =
        pow(
            saturate(
                dot(
                    normal,
                    halfVector)),
            SpecularPower);


    // 影ではスペキュラを消す
    specular *= ramp;


    finalColor +=
        SpecularColor *
        specular *
        SpecularIntensity;
    
    
    float finalAlpha =
        textureColor.a *
        DiffuseColor.a;

    return float4(
        finalColor,
        finalAlpha);
}