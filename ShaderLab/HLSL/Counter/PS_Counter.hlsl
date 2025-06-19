#include "Counter.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.Tex;
    float w = width.x;
    float h = height.x;
    uv.x /= w;
    uv.y /= h;
    float count2 = floor(count.x);
    
    uv.x += (1.0f / w) * (count2 % w);
    uv.y += (1.0f / h) * (int) (count2 / w);
    
    float4 color = tex.Sample(samLinear, uv);
    
   
    // アルファ値が0の場合はピクセルを非表示にする
    float alphaMask = step(0.0f, color.a); // アルファ値が0なら0、それ以外は1になる
    color *= alphaMask; // アルファが0のピクセルを完全に無効化
   // アルファ値を0から1の範囲に制限し、色に掛け合わせる
   
    
    return color;
}
