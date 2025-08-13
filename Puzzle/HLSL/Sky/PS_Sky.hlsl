#include "Header.hlsli"
float4 main(PS_Input input) : SV_Target0
{
    // 球面座標（法線ベクトル）の正規化
    float3 n = normalize(input.NormalWS);
    // 球面→UV
    // 経度、緯度をUVへ
    float u = atan2(n.z, n.x) / (2.0 * 3.14159265) + 0.5;
    float v = 1.0 - (acos(n.y) / 3.14159265);
    // テクスチャサンプリング
    float4 color = Texture.Sample(Sampler, float2(u, v));
    return color;
}