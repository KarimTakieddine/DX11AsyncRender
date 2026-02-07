struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

float4 Main(PSInput input) : SV_TARGET
{
    // The GPU interpolates the vertex colors automatically
    return input.color;
}