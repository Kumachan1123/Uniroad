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

float3 HSVtoRGB(float3 hsv)
{
    float3 rgb = clamp(abs(fmod(hsv.x * 6.0 + float3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return hsv.z * lerp(float3(1.0, 1.0, 1.0), rgb, hsv.y);
}

float3 GetRainbow(float cptime)
{
    float hue = fmod(cptime * 0.25, 1.0); // ���ԂɊ�Â���Hue��ω�
    return HSVtoRGB(float3(hue, 1.0, 1.0)); // HSV����RGB�ɕϊ�
}

