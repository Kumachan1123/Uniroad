#include "MaterialNoTex.hlsli"
///*
//    複数のHLSLファイルで共通で使用するパラメータ
//    テクスチャなしバージョン（環境マップのみ使用）
//*/

//// 環境マップのみ定義（メタリック反射用）
//TextureCube EnvMap : register(t10);
//SamplerState EnvSampler : register(s10);


//cbuffer Parameters : register(b0)
//{
//    float4 DiffuseColor : packoffset(c0);
//    float3 EmissiveColor : packoffset(c1);
//    float3 SpecularColor : packoffset(c2);
//    float SpecularPower : packoffset(c2.w);

//    float3 LightDirection[3] : packoffset(c3);
//    float3 LightDiffuseColor[3] : packoffset(c6);
//    float3 LightSpecularColor[3] : packoffset(c9);

//    float3 EyePosition : packoffset(c12);

//    float3 FogColor : packoffset(c13);
//    float4 FogVector : packoffset(c14);

//    float4x4 World : packoffset(c15);
//    float3x3 WorldInverseTranspose : packoffset(c19);
//    float4x4 WorldViewProj : packoffset(c22);
//};

///*
//    共通で使用するパラメータ
//*/

//// ライト用定数バッファ
//cbuffer LightParameters : register(b1)
//{
//    float4x4 lightViewProjection : packoffset(c0);
//    float4 lightPosition : packoffset(c4);
//    float3 lightDirirection : packoffset(c5);
//    float3 lightAmbient : packoffset(c6);
//};

//// 頂点シェーダ入力用

//// ピクセルシェーダ入力用（テクスチャなしバージョン）
//struct PS_Input
//{
//    float4 PositionWS : TEXCOORD0;
//    float3 NormalWS : TEXCOORD1;
//    float4 Diffuse : COLOR0;
//};


// 上でグローバルに定義されている定数をそのまま使います。
float Metallic = 1.0;
float Roughness = 0.1;
float3 F0_default = float3(0.04, 0.04, 0.04);
float emissiveStrength = .5;
static const float kAmbientIBL = 0.15f; // 簡易環境拡散の強さ（微調整可）
static const float kMaxEnvMipLevel = 8.0f; // 環境マップの最大Mipレベル（256x256なら8）

// Schlick の近似フレネル
float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// GGX Normal Distribution Function
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265359 * denom * denom;

    return nom / max(denom, 0.0000001);
}

// Geometry Function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / max(denom, 0.0000001);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float4 main(PS_Input input) : SV_TARGET0
{
    //// テクスチャを使わないので、DiffuseColorから直接アルベドを取得
    //float3 albedo = DiffuseColor.rgb;

    //// ノーマルマップを使わないので、頂点法線を使用
    //float3 normal = normalize(input.NormalWS);

    //// AOは遮蔽なし（1.0）
    //float ao = 1.0f;

    //// ラフネスは定数値を使用
    //float roughness = Roughness;
    //roughness = max(roughness, 0.01);

    //float3 viewDir = normalize(EyePosition - input.PositionWS.xyz);

    //// 簡易ディレクショナルライト
    //float3 lightDir = normalize(-lightDirirection); // ライトの方向
    //float3 halfVec = normalize(viewDir + lightDir);

    //// Metallic workflow: F0をMetallicに基づいて計算
    //float3 F0 = lerp(F0_default, albedo, Metallic);

    //// 拡散反射と鏡面反射の計算（Cook-Torrance BRDF）
    //float NdotL = max(dot(normal, lightDir), 0.0);
    //float NdotV = max(dot(normal, viewDir), 0.0);

    //// Specular BRDF
    //float D = DistributionGGX(normal, halfVec, roughness);
    //float G = GeometrySmith(normal, viewDir, lightDir, roughness);
    //float3 F = FresnelSchlick(max(dot(halfVec, viewDir), 0.0), F0);

    //float3 numerator = D * G * F;
    //float denominator = 4.0 * NdotV * NdotL;
    //float3 specular = numerator / max(denominator, 0.001);

    //// エネルギー保存則
    //float3 kS = F;
    //float3 kD = (1.0 - kS) * (1.0 - Metallic);

    //// 拡散反射（Lambert）
    //float3 diffuse = kD * albedo / 3.14159265359;

    //// 最終的なライティング
    //float3 directLight = (diffuse + specular) * NdotL * float3(1.0, 1.0, 1.0); // ライトカラーは白

    //// 環境光（アンビエント）- AOで遮蔽を考慮
    //float3 ambient = lightAmbient * albedo * kAmbientIBL * ao;

    //// 発光はEmissiveColorを直接使用
    //float3 emissive = EmissiveColor * emissiveStrength;

    //// 最終カラー = ディレクショナルライト + アンビエント + 発光（環境マップなし）
    //float3 color = directLight + emissive + ambient;
    
    //return float4(color, DiffuseColor.a);
    
    float3 normal = normalize(input.NormalWS);
    float3 toLight = normalize(-LightDirection[0]);
    float NdotL = dot(normal, toLight);
    float zeroL = step(0.0f, NdotL);
    float lightAmount = saturate(NdotL) * zeroL;
    float3 diffuse = mul(lightAmount,
            LightDiffuseColor[0] * DiffuseColor.rgb + EmissiveColor);
    
    float3 toEye = normalize(EyePosition - input.PositionWS.xyz);
    float3 halfVector = normalize(toLight + toEye);
    float3 NdotH = dot(normal, halfVector);
    float3 specular = pow(saturate(NdotH), SpecularPower) * LightSpecularColor[0];
    return float4(diffuse + specular, DiffuseColor.a);
}
