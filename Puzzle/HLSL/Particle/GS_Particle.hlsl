#include "Particle.hlsli"


StructuredBuffer<GPU_Particle> particles : register(t0); // SRV
//	4�̒��_���쐬����
static const int vnum = 4;

//	�e���_�̃x�[�X�Ƃ�����
static const float4 offset_array[vnum] =
{
    float4(-0.5f, 0.5f, 0.0f, 0.0f), // ����
	float4(0.5f, 0.5f, 0.0f, 0.0f), // �E��
	float4(-0.5f, -0.5f, 0.0f, 0.0f), // ����
	float4(0.5f, -0.5f, 0.0f, 0.0f), // �E��

};

[maxvertexcount(4)]
void main(point PS_INPUT input[1], inout TriangleStream<PS_INPUT> output)
{
    uint idx = input[0].ParticleIndex;
    float3 center = input[0].Pos.xyz;
    float2 scale = float2(1.0f, 1.0f); // �K�v�Ȃ� input[0].scale.xy ���ɕύX
    // �K�v�Ȃ� input[0] �� scale ����n���Ă���

    for (int i = 0; i < 4; i++)
    {
        PS_INPUT element = input[0];

        // �I�t�Z�b�g���X�P�[��
        float3 offset = float3(offset_array[i].xy * scale, 0);

        // ���S+�I�t�Z�b�g�����[���h�ϊ�
        float4 worldPos = float4(center + offset, 1.0f);
        worldPos = mul(worldPos, matWorld);

        // �r���[�E�ˉe�ϊ�
        element.Pos = mul(worldPos, matView);
        element.Pos = mul(element.Pos, matProj);

        // UV
        element.Tex.x = offset_array[i].x + 0.5f;
        element.Tex.y = -offset_array[i].y + 0.5f;

        output.Append(element);
    }
    output.RestartStrip();
}