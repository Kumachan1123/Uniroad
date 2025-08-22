//	C++から受け取るデータ
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; //    ワールド行列
    matrix matView; //    ビュー行列
    matrix matProj; //    プロジェクション行列
    float4 color; //   色
    float4 time; //    時間
    float4 padding; //    パディング
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

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
	
	//	渡された座標に、渡されたWorld、View、Projをクロス積していく。
	//	もし、C++側で既に計算された結果を使いたい場合、以下は邪魔となるため注意する
    output.Pos = mul(float4(input.Pos, 1), matWorld);
    //output.Pos = mul(output.Pos, matView);
    //output.Pos = mul(output.Pos, matProj);

	//	UV座標はそのまま渡す
    output.Tex = input.Tex;
    return output;
}