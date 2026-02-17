#include "Material.hlsli"

float4 main(PS_Input input) : SV_TARGET
{
    // テクスチャカラー
    float4 texColor = Texture.Sample(Sampler, input.TexCoord);

    // --- ここからライトパラメータをHLSL内部で直接定義 ---
    float3 lightDir = normalize(float3(0.577, -0.577, -0.577)); // 任意の方向に
    float3 lightColor = float3(1.0, 1.0, 1.0); // 白色ライト
    float ambientStrength = 0.2; // 環境光（弱く）
    float3 ambient = ambientStrength * lightColor;
    // --- ここまでをお好みで調整可能 ---

    // 法線の正規化
    float3 normal = normalize(input.NormalWS);

    // 拡散反射成分
    float diff = max(dot(normal, -lightDir), 0.0); // -lightDirでライト方向に
    float3 diffuse = diff * lightColor;

    // 環境光 + 拡散光
    float3 finalColor = ambient + diffuse;

    // 結果
    return float4(texColor.rgb * finalColor, texColor.a);
}