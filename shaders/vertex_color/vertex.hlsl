// Input structure from the vertex buffer
struct VSInput
{
    float3 position : POSITION; // Vertex Position (X, Y, Z)
    float3 color : COLOR0; // Vertex Color (R, G, B)
};

// Output structure to the pixel shader
struct VSOutput
{
    float4 position : SV_POSITION; // Transformed Position (required)
    float4 color : COLOR0; // Interpolated Color
};

// Optional: Matrix for transformation (if you are doing 3D/2D projection)
// cbuffer MatrixBuffer : register(b0) { matrix wvp; };

VSOutput Main(VSInput input)
{
    VSOutput output = (VSOutput) 0;

    // 1. Convert float3 position to float4 (required for SV_POSITION)
    // Setting W to 1.0f represents a 3D point
    output.position = float4(input.position, 1.0f);
    
    // If you have a WVP matrix, you would do:
    // output.position = mul(float4(input.position, 1.0f), wvp);

    // 2. Pass the color through
    // Convert float3 color to float4 (Alpha = 1.0f)
    output.color = float4(input.color, 1.0f);

    return output;
}