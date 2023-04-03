struct VS_IN
{
	//w = 1になっている
	float4 pos : POSITION;
	float4 nor : NORMAL;
	float2 tex : TEXCOORD0;
    //インスタンスごとに渡されるデータ
	column_major float4x4 world : MATRIX;
	float4 color : COLOR;
	uint InstanceId : SV_InstanceID;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 posw : POSITION0;
	float4 color : COLOR;
	float3 norw : NORMAL0;
	float2 tex : TEXCOORD0;
};

cbuffer ConstantBuffer
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.posw = mul(input.pos, input.world);
	output.pos = mul(output.posw, View);
	output.pos = mul(output.pos, Projection);

	//w = 0にすることで平行移動を無効化
	float4 nor = float4(input.nor.xyz, 0.0f);
	output.norw = normalize(mul(nor, World).xyz);
	
	output.tex = input.tex;
	output.color = input.color;

	return output;
}