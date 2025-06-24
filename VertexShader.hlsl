struct VS_INPUT
{
    float4 position : POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
    float3 worldPos : TEXCOORD0;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;        
    row_major float4x4 m_view;         
    row_major float4x4 m_proj;         

    float m_angle;                     
    float3 padding1;

    float fogStart;                   
    float fogEnd;                     
    float2 padding2;

    float3 fogColor;                 
    float fogDensity; 
    float3 cameraPosition;  

    int fogState;
    int culling;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;


    //For Scaling #3
    //float scale = lerp(1.0f, 0.25f, (sin(m_angle) + 1.0f) / 2.0f);
    //row_major float4x4 scaleMatrix = (row_major float4x4)0;
    //scaleMatrix._11 = scale;
    //scaleMatrix._22 = scale;
    //scaleMatrix._33 = scale;
    //scaleMatrix._44 = 1.0f;
    //input.position = mul(input.position, scaleMatrix);

    output.position = mul(input.position,m_world);
    output.position = mul(output.position,m_view);
    output.position = mul(output.position,m_proj);

    output.color = input.color;
    output.color1 = input.color1;

    output.worldPos = mul(input.position,m_world).xyz;

    // For moving the cube #3
    //Cube Position is at (-1.0f, -1.0f, 0.0f)
    //float4 position1 = mul(input.position + float4(2.5,2.5,0,0),m_world);
    //position1 = mul(position1,m_view);
    //position1 = mul(position1,m_proj);

    //output.position = lerp(output.position, position1, (sin(m_angle) + 1.0f) / 2.0f);

    
    return output;
}
