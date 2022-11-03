////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float3 lightDirection;
	float padding;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	// 이 텍스처 좌표 위치에서 샘플러를 사용하여 텍스처에서 픽셀 색상을 샘플링합니다.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	// 계산을 위해 빛 방향을 반전시킵니다.
	lightDir = -lightDirection;

	// 이 픽셀의 빛의 양을 계산합니다.
	lightIntensity = saturate(dot(input.normal, lightDir));

	// Determine the final amount of diffuse color based on the texture color combined with the light intensity.
	color = saturate(textureColor * lightIntensity);

    return color;
}