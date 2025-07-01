#include "Tile.hlsli"
Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);
// HSVからRGBに変換する関数
float3 HSVtoRGB(float3 hsv)
{
    float3 rgb = clamp(abs(fmod(hsv.x * 6.0 + float3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return hsv.z * lerp(float3(1.0, 1.0, 1.0), rgb, hsv.y);
}

float4 main(PS_INPUT input) : SV_TARGET
{
 
    // テクスチャをサンプリング
    float4 output = tex.Sample(samLinear, input.Tex);
    
 
    // 結果を返す
    return output;
}
