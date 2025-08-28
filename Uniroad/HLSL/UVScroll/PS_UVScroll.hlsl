#include "UVScroll.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // 横スクロールの速度（1.0で1秒に1枚分スクロール。調整可能）
    float scrollSpeed = 0.2f;
    // 時間経過でUVを横方向にオフセット
    float2 uv = input.Tex;
    uv.x += time.x * scrollSpeed;
    // 繰り返し（ループ）させる場合は frac を使う
    uv.x = frac(uv.x);
    float4 color = tex.Sample(samLinear, uv);
    // アルファ値が0ならピクセルを非表示
    float alphaMask = step(0.0f, color.a);
    color *= alphaMask;
    return color;
}
