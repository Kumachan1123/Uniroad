//	C++����󂯎��f�[�^
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; //    ���[���h�s��
    matrix matView; //    �r���[�s��
    matrix matProj; //    �v���W�F�N�V�����s��
    float4 color; //   �F
    float4 HP; //   x:���݂�HP, y:�ő�HP
};

//	���̃V�F�[�_���󂯎������̓��e
struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

//	�s�N�Z���V�F�[�_�֓n�������̓��e�Breturn����l�ƂȂ�
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};