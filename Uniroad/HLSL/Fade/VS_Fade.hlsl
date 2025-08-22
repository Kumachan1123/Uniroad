//	C++から受け取るデータ
cbuffer ConstBuffer : register(b0)
{
    float power; // 強度
    float fadeAmount; // フェードの進行度（0.0～1.0）
    float padding; // パディング
    int num; // テクスチャ番号
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
    output.Pos = mul(float4(input.Pos, 1), 1);
   
	//	UV座標はそのまま渡す
    output.Tex = input.Tex;
    return output;
}