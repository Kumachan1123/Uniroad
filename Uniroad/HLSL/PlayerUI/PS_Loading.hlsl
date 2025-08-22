#include "PlayerUI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.Tex;

    
    // �`�抄�� (0.0 �` 1.0 �͈̔͂Ő���)
    float drawLimit = renderRatio;
    
    // �������݂�UV���W���`��͈͊O�Ȃ� discard �ŕ`�悵�Ȃ�
    if (uv.x > drawLimit)
    {
        discard;
    }
    
    // �e�N�X�`���̃T���v�����O
    float4 color = tex.Sample(samLinear, uv);
    
    // �A���t�@�}�X�N�K�p�i���������������j
    float alphaMask = step(0.0f, color.a);
    color *= alphaMask;
    
    return color;
}