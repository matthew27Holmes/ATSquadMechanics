Texture2D<float4>  shaderTexture[7];

SamplerState SampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	uint textureID : BLENDINDICES0;
};

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
if (input.textureID == 0)
{
	textureColor = shaderTexture[0].Sample(SampleType, input.tex);
}
else if (input.textureID == 1)
{
	textureColor = shaderTexture[1].Sample(SampleType, input.tex);
}else if (input.textureID == 2)
{
	textureColor = shaderTexture[2].Sample(SampleType, input.tex);
}else if (input.textureID == 3)
{
	textureColor = shaderTexture[3].Sample(SampleType, input.tex);
}else if (input.textureID == 4)
{
	textureColor = shaderTexture[4].Sample(SampleType, input.tex);
}else if (input.textureID == 5)
{
	textureColor = shaderTexture[5].Sample(SampleType, input.tex);
}else if (input.textureID == 6)
{
	textureColor = shaderTexture[6].Sample(SampleType, input.tex);
}
else
{
	textureColor = float4(1, 0, 0, 1);
}
	return textureColor;
}