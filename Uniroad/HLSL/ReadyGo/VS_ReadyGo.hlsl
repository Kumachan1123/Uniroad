//	C++����󂯎��f�[�^
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; //    ���[���h�s��
    matrix matView; //    �r���[�s��
    matrix matProj; //    �v���W�F�N�V�����s��
    float4 color; //   �F
    float4 time; //    ����
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

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
	
 
    output.Pos = mul(float4(input.Pos, 1), matWorld);
    

	//	UV���W�͂��̂܂ܓn��
    output.Tex = input.Tex;
    return output;
}