#include "Stage.hlsli"
// C++������ݒ肳���f�[�^�A
Texture2D tex : register(t0); // �e�N�X�`��1
Texture2D tex2 : register(t1); // �e�N�X�`��2
SamplerState samLinear : register(s0); // �T���v���[�X�e�[�g


float4 main(PS_INPUT input) : SV_TARGET
{
    return tex.Sample(samLinear, input.Tex);
}