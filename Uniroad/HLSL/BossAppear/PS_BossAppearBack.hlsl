#include "Header.hlsli"

// C++側から設定されるデータ
//Texture2D tex : register(t0); // テクスチャ
SamplerState samLinear : register(s0); // サンプラーステート

// ピクセルシェーダーのメイン関数
float4 main(PS_INPUT input) : SV_TARGET
{
    // 時間経過で色を変える
    float4 output = float4(1.0f, 0.0f, 0.0f, 0.3f);
    output.r = sin(time.x * 6.0f) * 0.4f + 0.4f;
    output.a = sin(time.x * 6.0f) * 0.5f + 0.5f;
    output.a /= 2.0f; // 透明度を半分にする
    // 3秒以上経過したらアルファ値を0にする
    output.a *= step(time.x, 2.9f);
    
    return output;
}
