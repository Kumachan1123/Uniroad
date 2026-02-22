#include "Material.hlsli"

// 上でグローバルに定義されている定数をそのまま使います。
float Metallic = 1.0;
float Roughness = 0.1;
float3 F0_default = float3(0.04, 0.04, 0.04);

static const float kAmbientIBL = 0.15f; // 簡易環境拡散の強さ（微調整可）

// Schlick の近似フレネル
float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}




float4 main(PS_Input input) : SV_TARGET
{
  
    float4 texColor = Texture.Sample(Sampler, input.TexCoord);
    float3 albedo = texColor.rgb;

    float3 normal = normalize(input.NormalWS);
    float3 viewDir = normalize(EyePosition - input.PositionWS.xyz);
    float3 reflectDir = reflect(-viewDir, normal);

    // ここでキューブ左右反転（XZ符号反転）
    float3 reflectDirForEnv = float3(-reflectDir.x, reflectDir.y, -reflectDir.z);

    float3 envColor = EnvMap.Sample(EnvSampler, reflectDirForEnv).rgb;

    // フレネル等は省略（テスト目的）
    float3 color = envColor * albedo;
    return float4(color, 1.0);
}

