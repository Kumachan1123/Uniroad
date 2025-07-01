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
Texture2D tex : register(t0); // 背景テクスチャ
Texture2D killAll : register(t1); // 指示テクスチャ
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
   
    // 線の強度を計算
    float lineIntensity = sin(input.Tex.y * 200.0 + time.x * 10.0) * 0.05;

    // ノイズの強度を計算
    float noise = (frac(sin(dot(input.Tex.xy * float2(12.9898, 78.233), float2(12.9898, 78.233))) * 43758.5453) - 0.5) * 0.125;

    // ブラウン管効果を適用
    output.rgb += lineIntensity + noise;

    // 時間に基づいてグラデーションエフェクトを生成
    float gradient = exp(-abs(sin(time.x * 0.50 - input.Tex.y * 15.0)));
    
    // グラデーションエフェクトを加える
    output.rgb += gradient * float3(.5, .5, .5);
    // 背景
    float4 background = output * color;
    
  
    // 指示テクスチャの座標を計算
    float2 killAllUV = input.Tex;
 
    
    
    // 指示テクスチャをサンプリング
    float4 killAllColor = killAll.Sample(samLinear, killAllUV);
     
    killAllColor = float4(killAllColor.rgba);
    float4 clearColor = float4(0, 0, 0, 0);
    // 時間経過で指示テクスチャからLerpで透明に変える
    killAllColor = lerp(killAllColor, clearColor, cos(time.x * 1.1) + 0.65);
    background = lerp(background, clearColor, cos(time.x * 1.1) + 0.65);
    // 結果を返す
    return background + killAllColor;

}
