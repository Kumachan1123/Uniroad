// C++側から設定されるデータ①
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; // ワールド行列
    matrix matView; // ビュー行列
    matrix matProj; // プロジェクション行列
    float4 color; // 色
    float4 time; // 時間
   
};

// C++側から設定されるデータ②
Texture2D tex : register(t0); // テクスチャ1
SamplerState samLinear : register(s0); // サンプラーステート

// このシェーダが引数として受け取るデータ
struct PS_INPUT
{
    float4 pos : SV_POSITION; // 位置
    float2 Tex : TEXCOORD; // UV座標
};

float4 main(PS_INPUT input) : SV_TARGET
{
 
    // テクスチャをサンプリング
    float4 output = tex.Sample(samLinear, input.Tex);
 
    // 結果を返す
    return output;
}

