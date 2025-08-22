// C++側から設定されるデータ①
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; // ワールド行列
    matrix matView; // ビュー行列
    matrix matProj; // プロジェクション行列
    float4 color; // 色
    float4 time; // 時間
    float4 uv; // uv座標
    float4 alpha; // アルファ
};


// このシェーダが引数として受け取るデータ
struct PS_INPUT
{
    float4 pos : SV_POSITION; // 位置
    float2 Tex : TEXCOORD; // UV座標
};

// HSVからRGBに変換する関数
float3 HSVtoRGB(float3 hsv)
{
    float3 rgb = clamp(abs(fmod(hsv.x * 6.0 + float3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return hsv.z * lerp(float3(1.0, 1.0, 1.0), rgb, hsv.y);
}


// 赤から白へのグラデーションを作成するピクセルシェーダーのmain関数
float4 main(PS_INPUT input) : SV_TARGET
{
  // UV座標の中心を(0.5, 0.5)に設定する
    float2 center = uv.xy;
    
    // 現在のUV座標と中心との距離を計算する
    float2 uvOffset = input.Tex - center;
    float distance = length(uvOffset); // 距離

    // 光の弧の描画範囲を調整する
    float innerRadius = 0.04; // 色の面積の半径
    float outerRadius = 0.125; // 黒の面積の半径

    // 光の弧の範囲内でのグラデーションを計算する
    float corona = smoothstep(innerRadius, outerRadius, distance);

    // 補間する
    float3 rgb = lerp(color.rgb, float3(0.0, 0.0, 0.0), corona);

    // アルファ値を光の弧に基づいて調整する
    float a = (color.a - corona);

    // 色を返す（中心に近づくほど白くなるように赤色と白色を適用）
    return float4(rgb, a);
}
