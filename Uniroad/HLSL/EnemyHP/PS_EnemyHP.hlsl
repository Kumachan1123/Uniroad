#include "EnemyHP.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // �w�肳�ꂽ�摜�̕\��
    float4 output = tex.Sample(samLinear, input.Tex);

    // ����HP
    float nowHP = HP.x;
    // �ő�HP
    float maxHP = HP.y;
    // HP�̊���
    float hpRatio = nowHP / maxHP;

    // HP�̐F
    // �� �� ���F (0.5f�`1.0f)
    float greenToYellow = saturate((hpRatio - 0.5f) * 2.0f);

    // ���F �� ��F (0.25f�`0.5f)
    float yellowToOrange = saturate((hpRatio - 0.25f) * 4.0f) * (1.0f - greenToYellow);

    // ��F �� �ԐF (0.0f�`0.25f)
    float orangeToRed = saturate(hpRatio * 4.0f) * (1.0f - yellowToOrange - greenToYellow);

    // �e�F������
    float4 HPcolor =
        greenToYellow * float4(0.0f, 1.0f, 0.0f, 1.0f) + // �� �� ���F
        yellowToOrange * float4(1.0f, 1.0f, 0.0f, 1.0f) + // ���F �� ��F
        orangeToRed * float4(1.0f, 0.5f, 0.0f, 1.0f) + // ��F �� �ԐF
        (1.0f - greenToYellow - yellowToOrange - orangeToRed) * float4(1.0f, 0.0f, 0.0f, 1.0f); // ��

    // �o�͂̐F
    return output * HPcolor * color;
}
