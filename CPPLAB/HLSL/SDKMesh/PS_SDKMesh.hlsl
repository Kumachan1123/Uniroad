Texture2D textureMap : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャU座標を反転
    float2 fixedTexCoord = float2(1.0f - input.texCoord.x, input.texCoord.y);
    
    float4 texColor = textureMap.Sample(samplerState, fixedTexCoord);
    
    return float4(texColor.rgb, texColor.a);
}