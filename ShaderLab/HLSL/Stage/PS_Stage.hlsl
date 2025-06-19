#include "Stage.hlsli"
// C++側から設定されるデータ②
Texture2D tex : register(t0); // テクスチャ1
Texture2D tex2 : register(t1); // テクスチャ2
SamplerState samLinear : register(s0); // サンプラーステート


float4 main(PS_INPUT input) : SV_TARGET
{
    return tex.Sample(samLinear, input.Tex);
}