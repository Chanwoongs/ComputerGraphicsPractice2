// The first thing thats new in our effect file is a variable called SkyMap, which is a TextureCube
// resource. TextureCube is like the Texture2D, but instead of one texture, it holds an array of 6
// texture2D's, two for each axis.

struct Light
{
	float3 dir;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame
{
	Light light;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;
TextureCube SkyMap;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};

// This structure is used to send the information we need from the skymaps VS to the skymaps PS.
// We will only be needing the position and texture coordinates in our PS.
struct SKYMAP_VS_OUTPUT	// output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VS_OUTPUT output;

	output.Pos = mul(inPos, WVP);

	output.normal = mul(normal, World);

	output.TexCoord = inTexCoord;

	return output;
}

// Here is our skymaps VS. As you can see, we are taking in normal values for each vertex still, 
// this is because we are going to be using the same input layout for all our geometry(so we don't
// have to do more code). However, we will not be using the normals. And in fact, we did not define
// texture coordinates for our vertices either. This is because the texture coordinates will be 
// defined by our vertices position. We can use our vertices position as a vector, describing the 
// texel in our texturecube	to color the pixel with. You can see how we do that with the line 
// output.texCoord = inPos;. Also, notice when we are defining the output position, we take the 
// values.xyww instead of.xyzw. This is because	w is equal to 1.0f. We want to make sure our 
// skymap is always the furthest object in our scene, so we	want to set our z value to 1.0f too, 
// which is what w is. Remember, 1.0f is the furthest value from the screen.
SKYMAP_VS_OUTPUT SKYMAP_VS(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;

	// Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.Pos = mul(float4(inPos, 1.0f), WVP).xyww;

	output.texCoord = inPos;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float4 diffuse = ObjTexture.Sample( ObjSamplerState, input.TexCoord );

	float3 finalColor;

	finalColor = diffuse * light.ambient;
	finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);

	return float4(finalColor, diffuse.a);
}

// The only thing thats left to do now is create our skymaps PS. This is very easy. All we have to
// do is sample the texturecube using the 3D texture coordinate passed in from the VS, and return 
// the color value of that texel.
float4 SKYMAP_PS(SKYMAP_VS_OUTPUT input) : SV_Target
{
	return SkyMap.Sample(ObjSamplerState, input.texCoord);
}

float4 D2D_PS(VS_OUTPUT input) : SV_TARGET
{
	float4 diffuse = ObjTexture.Sample( ObjSamplerState, input.TexCoord );

	return diffuse;
}
