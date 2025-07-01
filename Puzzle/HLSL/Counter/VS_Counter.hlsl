#include "Counter.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    // ワールド座標 → ビュー座標 → 投影座標の変換
    float4 worldPosition = mul(float4(input.Pos, 1.0), matWorld);
    float4 viewPosition = mul(worldPosition, matView);
    output.Pos = mul(viewPosition, matProj);
    output.Tex = input.Tex;
    return output;
}