cbuffer TransformBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 worldPos = mul(float4(input.position, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    output.position = mul(viewPos, projection);
    
    output.normal = mul(input.normal, (float3x3) world);
    
    // テクスチャU座標を反転
    output.texCoord = float2(1.0f - input.texCoord.x, input.texCoord.y);
    
    return output;
}