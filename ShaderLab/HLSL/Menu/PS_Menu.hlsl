#include "Menu.hlsli"

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
    // 時間に基づいてグラデーションエフェクトを生成
    float gradient = exp(-abs(sin(time * 1.0 + input.Tex.y * 5.0)));
    
    // テクスチャをサンプリング
    float4 output = tex.Sample(samLinear, input.Tex);
    
    // colorの全成分が0かどうかを判定
    float isColorZero = step(0.0001, dot(color.rgb, color.rgb)); // colorが(0,0,0)なら0、そうでなければ1
    
    // HSV値を時間とテクスチャ座標に基づいて生成
    float3 hsv = float3(fmod(time * 0.5 + input.Tex.y, 1.0), 1.0, 1.0); // Hueの変化で虹色を生成
    float3 rgb = HSVtoRGB(hsv); // RGBに変換
    
    // 発光色を混ぜる
    output.rgb += gradient * lerp(rgb, color.rgb, isColorZero); // colorが0なら虹色、そうでなければ指定の色を使う
    
    // 結果を返す
    return output;
}

