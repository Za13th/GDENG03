struct PS_INPUT
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


float saturate(float x)
{
    return clamp(x, 0.0, 1.0);
}

float4 psmain(PS_INPUT input) : SV_Target
{
        if(fogState == 0)
        return float4(input.color,1.0f);

        float distance = length(cameraPosition - input.worldPos);

        float fogFactor = (fogEnd - distance) / (fogEnd - fogStart);
        

        //for exponential fog
        if(fogState == 2)
        fogFactor = exp(-distance * fogDensity);  
        // for exponential squared fog
        else if(fogState == 3)
        fogFactor = exp(-pow(fogDensity * distance, 2.0));

        fogFactor = saturate(fogFactor); // Clamp between 0 and 1

        float3 baseColor = input.color;
        if(all(baseColor.xyz == fogColor.xyz)) 
        {
            baseColor.x += 0.01f; // Slightly adjust to avoid discarding
            baseColor.y += 0.01f; 
            baseColor.z += 0.01f; 
        }

        float3 finalColor = lerp(fogColor, baseColor, fogFactor);

        if (culling == 1 && all(finalColor.xyz == fogColor.xyz)) 
        discard; 


        return float4(finalColor, 1.0f);
        //return float4(input.color,1.0f);
        //return float4(lerp(input.color, input.color1, (sin(m_angle) + 1.0f) / 2.0f),1.0f);
}