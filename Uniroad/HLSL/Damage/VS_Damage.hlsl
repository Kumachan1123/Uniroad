//	C++����󂯎��f�[�^
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; //    ���[���h�s��
    matrix matView; //    �r���[�s��
    matrix matProj; //    �v���W�F�N�V�����s��
    float4 color; //   �F
    float4 time; //    ����
    float4 padding; //    �p�f�B���O
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
	
	//	�n���ꂽ���W�ɁA�n���ꂽWorld�AView�AProj���N���X�ς��Ă����B
	//	�����AC++���Ŋ��Ɍv�Z���ꂽ���ʂ��g�������ꍇ�A�ȉ��͎ז��ƂȂ邽�ߒ��ӂ���
    output.Pos = mul(float4(input.Pos, 1), matWorld);
    //output.Pos = mul(output.Pos, matView);
    //output.Pos = mul(output.Pos, matProj);

	//	UV���W�͂��̂܂ܓn��
    output.Tex = input.Tex;
    return output;
}