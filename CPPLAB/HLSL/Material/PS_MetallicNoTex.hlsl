#include "MaterialNoTex.hlsli"
///*
//    複数のHLSLファイルで共通で使用するパラメータ
//    テクスチャなしバージョン（環境マップのみ使用）
//*/




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

float4 main(PS_Input input) : SV_TARGET0
{
    float3 normal = normalize(input.NormalWS);
    float3 viewDir = normalize(EyePosition - input.PositionWS.xyz);
    float3 toLight = normalize(-LightDirection[0]);
    float NdotV = max(dot(normal, viewDir), 0.0);
    float NdotL = dot(normal, toLight);
    float zeroL = step(0.0f, NdotL);
    float lightAmount = saturate(NdotL) * zeroL;
    
    // ディレクショナルライト
    float3 diffuse = mul(lightAmount,
            LightDiffuseColor[0] * DiffuseColor.rgb + EmissiveColor);
    
    float3 halfVector = normalize(toLight + viewDir);
    float3 NdotH = dot(normal, halfVector);
    float3 specular = pow(saturate(NdotH), SpecularPower) * LightSpecularColor[0];
    
    // 環境マップの反射計算
    float3 F0 = lerp(F0_default, DiffuseColor.rgb, Metallic);
    float3 F = FresnelSchlick(NdotV, F0);
    
    // 反射方向
    float3 reflectDir = reflect(-viewDir, normal);
    
    // ラフネスに応じたMipレベルの計算
    float mipLevel = Roughness * kMaxEnvMipLevel;
    
    // 環境マップからの反射サンプリング
    float3 envReflection = EnvMap.SampleLevel(EnvSampler, reflectDir, mipLevel).rgb;
    
    // 環境拡散（ambient IBL）
    // 法線方向に基づいた環境マップのサンプリング
    float3 envDiffuse = EnvMap.SampleLevel(EnvSampler, normal, kMaxEnvMipLevel).rgb;
    
    // メタリック材質でのエネルギー保存
    float3 kS = F; // Fresnel = 鏡面反射の割合
    float3 kD = (1.0 - kS) * (1.0 - Metallic);
    
    // 環境光の寄与
    float3 ambientDiffuse = envDiffuse * kD * DiffuseColor.rgb * kAmbientIBL;
    float3 ambientSpecular = envReflection * kS;
    
    float3 color = diffuse + specular + ambientDiffuse + ambientSpecular;
    
    return float4(color, DiffuseColor.a);
}
