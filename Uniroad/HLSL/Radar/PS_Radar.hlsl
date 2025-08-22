// C++������ݒ肳���f�[�^�@
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; // ���[���h�s��
    matrix matView; // �r���[�s��
    matrix matProj; // �v���W�F�N�V�����s��
    float4 color; // �F
    float4 time; // ����
   
};

// C++������ݒ肳���f�[�^�A
Texture2D tex : register(t0); // �e�N�X�`��1
SamplerState samLinear : register(s0); // �T���v���[�X�e�[�g

// ���̃V�F�[�_�������Ƃ��Ď󂯎��f�[�^
struct PS_INPUT
{
    float4 pos : SV_POSITION; // �ʒu
    float2 Tex : TEXCOORD; // UV���W
};

float4 main(PS_INPUT input) : SV_TARGET
{
   
    
   
     // �s�N�Z���̒��S����̋������v�Z�i���a1�̉~�`�ɂ����鋗���j
    float distanceFromCenter = length(input.Tex - float2(0.5, 0.5));

    // �O���f�[�V�����G�t�F�N�g�̋��x�������Ɋ�Â��Ē�������
    float gradient = exp(-abs(sin(time.x * .050 + distanceFromCenter * -.050)));

    // �e�N�X�`�����T���v�����O
    float4 output = tex.Sample(samLinear, input.Tex);
   // output.a = 0;
    
    // �O���f�[�V�����G�t�F�N�g��������
    // �G�t�F�N�g�̐F�͉摜�̐F�ɉ��Z
    output.rgb += gradient * color.rgb;

    // ���ʂ�Ԃ�
    return output;
}

