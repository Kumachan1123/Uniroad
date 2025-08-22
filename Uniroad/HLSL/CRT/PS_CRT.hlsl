#include "CRT.hlsli"

// C++������ݒ肳���f�[�^�A
Texture2D tex : register(t0); // �e�N�X�`��1
SamplerState samLinear : register(s0); // �T���v���[�X�e�[�g



 

float4 main(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`�����T���v�����O
    float4 output = tex.Sample(samLinear, input.Tex);
   
    // ���̋��x���v�Z
    float lineIntensity = sin(input.Tex.y * 200.0 + time.x * 10.0) * 0.05;

    // �m�C�Y�̋��x���v�Z
    float noise = (frac(sin(dot(input.Tex.xy * float2(12.9898, 78.233), float2(12.9898, 78.233))) * 43758.5453) - 0.5) * 0.125;

    // �u���E���ǌ��ʂ�K�p
    output.rgb += lineIntensity + noise;

    // ���ԂɊ�Â��ăO���f�[�V�����G�t�F�N�g�𐶐�
    float gradient = exp(-abs(sin(time.x * 1.0 - input.Tex.y * 3.0)));
    
    // �O���f�[�V�����G�t�F�N�g��������
    output.rgb += gradient * float3(.5, .5, .5);
   
    // ���ʂ�Ԃ�
    return output * color;
}
