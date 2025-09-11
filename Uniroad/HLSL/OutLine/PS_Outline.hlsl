#include "Common.hlsli"
#include "Params.hlsli"

// メイン関数
float4 main(PS_Input input) : SV_Target0
{
    // 法線と視線方向の角度を計算
    float3 viewDirection = normalize(EyePosition - input.PositionWS);
    float edgeFactor = 1.0 - dot(normalize(input.NormalWS), viewDirection);

    // エッジ部分をグラデーションで描画
    edgeFactor = smoothstep(0.0, .9, edgeFactor);
    float4 textureColor = Texture.Sample(Sampler, input.TexCoord); // テクスチャをサンプル
    // アウトライン色にフェード効果を適用
    float alpha = pow(edgeFactor, 1) * OutlineColor.a;
    return float4(OutlineColor.rgba);
}