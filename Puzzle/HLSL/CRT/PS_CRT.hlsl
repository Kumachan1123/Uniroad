#include "CRT.hlsli"

// C++側から設定されるデータ②
Texture2D tex : register(t0); // テクスチャ1
SamplerState samLinear : register(s0); // サンプラーステート



 

float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャをサンプリング
    float4 output = tex.Sample(samLinear, input.Tex);
   
    // 線の強度を計算
    float lineIntensity = sin(input.Tex.y * 200.0 + time.x * 10.0) * 0.05;

    // ノイズの強度を計算
    float noise = (frac(sin(dot(input.Tex.xy * float2(12.9898, 78.233), float2(12.9898, 78.233))) * 43758.5453) - 0.5) * 0.125;

    // ブラウン管効果を適用
    output.rgb += lineIntensity + noise;

    // 時間に基づいてグラデーションエフェクトを生成
    float gradient = exp(-abs(sin(time.x * 1.0 - input.Tex.y * 3.0)));
    
    // グラデーションエフェクトを加える
    output.rgb += gradient * float3(.5, .5, .5);
   
    // 結果を返す
    return output * color;
}
