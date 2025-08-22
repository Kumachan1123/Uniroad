#include "Header.hlsli"

// C++������ݒ肳���f�[�^
//Texture2D tex : register(t0); // �e�N�X�`��
SamplerState samLinear : register(s0); // �T���v���[�X�e�[�g

// �s�N�Z���V�F�[�_�[�̃��C���֐�
float4 main(PS_INPUT input) : SV_TARGET
{
    // ���Ԍo�߂ŐF��ς���
    float4 output = float4(1.0f, 0.0f, 0.0f, 0.3f);
    output.r = sin(time.x * 6.0f) * 0.4f + 0.4f;
    output.a = sin(time.x * 6.0f) * 0.5f + 0.5f;
    output.a /= 2.0f; // �����x�𔼕��ɂ���
    // 3�b�ȏ�o�߂�����A���t�@�l��0�ɂ���
    output.a *= step(time.x, 2.9f);
    
    return output;
}
