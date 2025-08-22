#include "Counter.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    // ���[���h���W �� �r���[���W �� ���e���W�̕ϊ�
    float4 worldPosition = mul(float4(input.Pos, 1.0), matWorld);
    float4 viewPosition = mul(worldPosition, matView);
    output.Pos = mul(viewPosition, matProj);
    output.Tex = input.Tex;
    return output;
}