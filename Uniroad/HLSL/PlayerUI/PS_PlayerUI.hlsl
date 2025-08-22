#include "PlayerUI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//	指定された画像の表示
    float4 output = tex.Sample(samLinear, input.Tex);

	// 画像の透過度を変更
    float smoothValue = renderRatio;
    output.a *= lerp(1.0f, 0.0f, smoothstep(smoothValue, smoothValue + 0.00000001f, input.Tex.x));
    output.rgb *= input.Color.rgb;

    return output;
}