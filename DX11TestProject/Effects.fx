struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VS(float4 inPos : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = inPos;
	output.color = color;

	return output;
}

float4 PS(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}