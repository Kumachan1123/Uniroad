#include "EnemyHP.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // 指定された画像の表示
    float4 output = tex.Sample(samLinear, input.Tex);

    // 今のHP
    float nowHP = HP.x;
    // 最大HP
    float maxHP = HP.y;
    // HPの割合
    float hpRatio = nowHP / maxHP;

    // HPの色
    // 緑 → 黄色 (0.5f～1.0f)
    float greenToYellow = saturate((hpRatio - 0.5f) * 2.0f);

    // 黄色 → 橙色 (0.25f～0.5f)
    float yellowToOrange = saturate((hpRatio - 0.25f) * 4.0f) * (1.0f - greenToYellow);

    // 橙色 → 赤色 (0.0f～0.25f)
    float orangeToRed = saturate(hpRatio * 4.0f) * (1.0f - yellowToOrange - greenToYellow);

    // 各色を合成
    float4 HPcolor =
        greenToYellow * float4(0.0f, 1.0f, 0.0f, 1.0f) + // 緑 → 黄色
        yellowToOrange * float4(1.0f, 1.0f, 0.0f, 1.0f) + // 黄色 → 橙色
        orangeToRed * float4(1.0f, 0.5f, 0.0f, 1.0f) + // 橙色 → 赤色
        (1.0f - greenToYellow - yellowToOrange - orangeToRed) * float4(1.0f, 0.0f, 0.0f, 1.0f); // 赤

    // 出力の色
    return output * HPcolor * color;
}
