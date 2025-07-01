//	C++から受け取るデータ
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; //    ワールド行列
    matrix matView; //    ビュー行列
    matrix matProj; //    プロジェクション行列
    float4 color; //   色
    float4 HP; //   x:現在のHP, y:最大HP
};

//	このシェーダが受け取る引数の内容
struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

//	ピクセルシェーダへ渡す引数の内容。returnする値となる
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};