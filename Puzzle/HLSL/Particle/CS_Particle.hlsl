#include "Particle.hlsli"


cbuffer ParticleSystemParams : register(b1)
{
    float3 globalForce;
    float deltaTime;
    // �K�v�Ȃ瑼�ɂ�
};


// �p�[�e�B�N���o�b�t�@
RWStructuredBuffer<GPU_Particle> particles : register(u0);
[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint idx = DTid.x;

    GPU_Particle p = particles[idx];

    // ������0�ȉ��Ȃ牽�����Ȃ��i����ł闱�q�j
    if (p.life <= 0.0f)
        return;

    // ��{�����i��F�d�͓K�p�A�ʒu�X�V���j
    p.velocity += globalForce * deltaTime;
    p.position += p.velocity * deltaTime;

    // ��������
    p.life -= deltaTime;

    // �^�C�v���ƕ�����\
    // if (p.type == 1) { ... }

    // �����ŐF��X�P�[���A��]�̕�Ԃ��i�K�v�Ȃ�j

    // �����߂�
    particles[idx] = p;
}