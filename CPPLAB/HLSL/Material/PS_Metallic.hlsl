#include "Material.hlsli"

// metallic/roughnessもcBufferや定数で指定可
float Metallic = 1.0; // 1.0 = メタル
float Roughness = 0.1; // ツルツル
float3 F0_default = float3(0.04, 0.04, 0.04); // 非金属F0（反射率）

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