#include "Particle.hlsli"

Texture2D tex : register(t0); // メインのテクスチャ
Texture2D colorTex : register(t1); // グラデーションやその他の色関連のテクスチャ

SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // 単純にテクスチャをサンプリング
    float4 color = colors;

    // 頂点で決めたカラフルな色を乗算
    color.rgb *= input.Color.rgb;
    color.a *= input.Color.a;

    return color;
}