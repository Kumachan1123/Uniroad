//	C++����󂯎��f�[�^
cbuffer ConstBuffer : register(b0)
{
    float power; // ���x
    float fadeAmount; // �t�F�[�h�̐i�s�x�i0.0�`1.0�j
    float padding; // �p�f�B���O
    int num; // �e�N�X�`���ԍ�
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
    output.Pos = mul(float4(input.Pos, 1), 1);
   
	//	UV���W�͂��̂܂ܓn��
    output.Tex = input.Tex;
    return output;
}