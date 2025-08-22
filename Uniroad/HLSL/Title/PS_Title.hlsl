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
Texture2D tex2 : register(t1); // �e�N�X�`��2
SamplerState samLinear : register(s0); // �T���v���[�X�e�[�g

// ���̃V�F�[�_�������Ƃ��Ď󂯎��f�[�^
struct PS_INPUT
{
    float4 pos : SV_POSITION; // �ʒu
    float2 Tex : TEXCOORD; // UV���W
};
// HSV����RGB�ɕϊ�����֐�
float3 HSVtoRGB(float3 hsv)
{
    float3 rgb = clamp(abs(fmod(hsv.x * 6.0 + float3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return hsv.z * lerp(float3(1.0, 1.0, 1.0), rgb, hsv.y);
}
float4 main(PS_INPUT input) : SV_TARGET
{
    // ���ԂɊ�Â��ăO���f�[�V�����G�t�F�N�g�𐶐�
    float gradient = exp(-abs(sin(time.x * 1.0 + input.Tex.y * 5.0)));
    
    // �e�N�X�`�����T���v�����O
    float4 output = tex.Sample(samLinear, input.Tex);
    
    // color�̑S������0���ǂ����𔻒�
    float isColorZero = step(0.001, dot(color.rgb, color.rgb)); // color��(0,0,0)�Ȃ�0�A�����łȂ����1
    
    // HSV�l�����Ԃƃe�N�X�`�����W�Ɋ�Â��Đ���
    float3 hsv = float3(fmod(time.x * 0.5 + input.Tex.y, 1.0), 1.0, 1.0); // Hue�̕ω��œ��F�𐶐�
    float3 rgb = HSVtoRGB(hsv); // RGB�ɕϊ�
    
    // �����F��������
    output.rgb += gradient * lerp(rgb, color.rgb, isColorZero); // color��0�Ȃ���F�A�����łȂ���Ύw��̐F���g��
    
    // ���ʂ�Ԃ�
    return output;
}

