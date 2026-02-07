// 頂点入力用
struct VS_Input
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

// ピクセルライティング用構造体
struct PS_Input
{
    float4 Position : SV_Position;
    float3 NormalWS : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float3 PositionWS : TEXCOORD1;
};
cbuffer cbOutline : register(b2)
{
 
    float4x4 ViewProj : packoffset(c0); // ワールドビュー射影行列（c0 ~ c3 を使用）
    float4 OutlineColor : packoffset(c4); // アウトラインの色（c4 を使用）
    float OutlineThickness : packoffset(c5.x); // アウトラインの太さ（c5 を使用）
    float FOVTheta : packoffset(c6.x); // FOVの半分の角度（c5 を使用）
    float2 padding : packoffset(c7.x); // パディング（c6 を使用）

}