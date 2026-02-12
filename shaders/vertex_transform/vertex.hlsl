cbuffer TransformBuffer : register(b0) // Matches slot 0
{
    row_major matrix modelMatrix;
};

// Input structure from the vertex buffer
struct VSInput
{
    float3 position : POSITION; // Vertex Position (X, Y, Z)
    float4 color : COLOR0; // Vertex Color (R, G, B, A)
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
    output.position = mul(float4(input.position, 1.0f), modelMatrix);
    
    // If you have a WVP matrix, you would do:
    // output.position = mul(float4(input.position, 1.0f), wvp);

    // 2. Pass the color through
    output.color = input.color;

    return output;
}