cbuffer Camera : register(b0)
{
	matrix View;
	matrix Projection;
};

cbuffer Object : register(b1)
{
	matrix World;
};

struct VSInput
{
	float3 Position : POSITION;
	float3 Color : COLOR0;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
};

PSInput VShader(VSInput Input)
{
	PSInput Output;

	Output.Color = float4(Input.Color, 1.0f);

	Output.Position = float4(Input.Position, 1.f);
	Output.Position = mul(Output.Position, World);
	Output.Position = mul(Output.Position, View);
	Output.Position = mul(Output.Position, Projection);

	return Output;
}

float4 PShader(PSInput Input) : SV_TARGET
{
	return Input.Color;
}

