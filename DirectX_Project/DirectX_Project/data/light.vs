////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////

/////////////
// DEFINES //
/////////////
#define NUM_LIGHTS 3

/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
	float padding; // GPU에 넘겨줄때 16byte 단위를 주기위해
};

cbuffer LightPositionBuffer
{
	float4 lightPosition[NUM_LIGHTS];
};

cbuffer FogBuffer
{
    float fogStart;
    float fogEnd;
    float padding1, padding2;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 instancePosition : TEXCOORD1;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
    float3 lightPos1 : TEXCOORD2;
    float3 lightPos2 : TEXCOORD3;
    float3 lightPos3 : TEXCOORD4;
    float fogFactor : FOG;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
	float4 worldPosition;
    float4 currCameraPosition;
    float angle = 0.0f;
    
    matrix <float, 4, 4> rotMatrix = { 
                  cos(angle), 0.0f, sin(angle), 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  -sin(angle), 0.0f, cos(angle), 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f
                  };

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    input.position.x += input.instancePosition.x;
    input.position.y += input.instancePosition.y;
    input.position.z += input.instancePosition.z;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, mul(rotMatrix, worldMatrix));
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)mul(rotMatrix, worldMatrix));
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, mul(rotMatrix, worldMatrix)); // Vertex를 world 좌표계로 바꿈

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz; // E를 구한다. 카메라 위치 - Vertex
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);

    // Determine the light positions based on the position of the lights and the position of the vertex in the world.
    output.lightPos1.xyz = lightPosition[0].xyz - worldPosition.xyz; // d 구하기
    output.lightPos2.xyz = lightPosition[1].xyz - worldPosition.xyz; // d 구하기
    output.lightPos3.xyz = lightPosition[2].xyz - worldPosition.xyz; // d 구하기

    // Normalize the light position vectors.
    output.lightPos1 = normalize(output.lightPos1);
    output.lightPos2 = normalize(output.lightPos2);
    output.lightPos3 = normalize(output.lightPos3);

    // Calculate the camera position.
    currCameraPosition = mul(input.position, worldMatrix);
    currCameraPosition = mul(currCameraPosition, viewMatrix);

    // Calculate linear fog.    
    output.fogFactor = saturate((fogEnd - currCameraPosition.z) / (fogEnd - fogStart));

    return output;
}