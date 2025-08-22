// C++側から設定されるデータ
cbuffer ConstBuffer : register(b0)
{
    float smoothness; // なめらかさ
    float fadeAmount; // フェードの進行度（0.0～1.0）
    float2 padding; // パディング
};

// テクスチャとサンプラー
Texture2D fadeTex : register(t0); // フェード
SamplerState samLinear : register(s0);

// シェーダー入力
struct PS_INPUT
{
    float4 pos : SV_POSITION; // 位置
    float2 Tex : TEXCOORD; // UV座標
};

// ディゾルブエフェクト用ピクセルシェーダー
float4 main(PS_INPUT input) : SV_TARGET
{
    // フェードテクスチャの明度を取得
    float fadeValue = fadeTex.Sample(samLinear, input.Tex).r;

    //// 各テクスチャをサンプリング
    //float4 blackSample = BlackTex.Sample(samLinear, input.Tex);
    //float4 readySample = readyTex.Sample(samLinear, input.Tex);
    //float4 goSample = goTex.Sample(samLinear, input.Tex);

    //// num に基づいてテクスチャを選択（線形補間またはステップ関数を使用）
    //float4 mainValue = lerp(blackSample, readySample, step(1, num));
    //mainValue = lerp(mainValue, goSample, step(2, num));

    // フェード処理の強度を元にブレンドファクターを計算
    float alpha = smoothstep(fadeAmount - smoothness, fadeAmount + smoothness, fadeValue);

    // テクスチャとフェード処理を合成
    return float4(0, 0, 0, alpha);
}
