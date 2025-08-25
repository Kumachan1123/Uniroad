#include "Particle.hlsli"

Texture2D tex : register(t0); // ���C���̃e�N�X�`��
Texture2D colorTex : register(t1); // �O���f�[�V�����₻�̑��̐F�֘A�̃e�N�X�`��

SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // �P���Ƀe�N�X�`�����T���v�����O
    float4 color = colors;

    // ���_�Ō��߂��J���t���ȐF����Z
    color.rgb *= input.Color.rgb;
    color.a *= input.Color.a;

    return color;
}