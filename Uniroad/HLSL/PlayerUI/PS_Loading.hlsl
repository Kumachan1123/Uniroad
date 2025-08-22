#include "PlayerUI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.Tex;

    
    // 描画割合 (0.0 ～ 1.0 の範囲で制御)
    float drawLimit = renderRatio;
    
    // もし現在のUV座標が描画範囲外なら discard で描画しない
    if (uv.x > drawLimit)
    {
        discard;
    }
    
    // テクスチャのサンプリング
    float4 color = tex.Sample(samLinear, uv);
    
    // アルファマスク適用（透明部分を消す）
    float alphaMask = step(0.0f, color.a);
    color *= alphaMask;
    
    return color;
}