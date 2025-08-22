#include "PlayerUI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャの色を取得
    float4 output = tex.Sample(samLinear, input.Tex);

    // UVを[-1,1]の範囲に変換（中心を (0.5,0.5) にする）
    float2 uv = (input.Tex - 0.5f) * 2.0f;

    // 回転の基準点
    float theta = atan2(uv.x, uv.y);

    // 角度を 0 ~ 1 の範囲に正規化（時計回り）
    float normalizedTheta = (theta + 3.14159265359f) / (2.0f * 3.14159265359f);

    // 透過の進行度に応じてアルファを変更（時計回りに透過）
    float smoothValue = renderRatio;
    output.a *= lerp(1.0f, 0.0f, smoothstep(smoothValue, smoothValue + 0.00000001f, normalizedTheta));

    // ゲージの色を変更
 // 色の補間（満タンなら青、それ以外は赤）
    float fullGauge = saturate(renderRatio); // renderRatioが1なら1.0、それ以外は0.0に近づく
    float4 fullColor = float4(0.0f, 0.5f, 1.0f, 1.0f); // 青色
    float4 emptyColor = float4(1.5f, 0.0f, 0.0f, 1.0f); // 赤色

    float4 gaugeColor = lerp(emptyColor, fullColor, fullGauge);
    float4 finalColor = float4(output * gaugeColor);
  
    return finalColor;
}