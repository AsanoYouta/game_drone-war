Texture2D tex : register(t0);
SamplerState smpr : register(s0);

cbuffer ConstantBuffer 
{
	float2 viewPort; //(width, height)
	float2 options;  //(offset, intensity)
	float gausWeight[8];
}

//閾値以上の輝度を持つ色のみ抽出
float3 GetHighBrightCol(float2 uv)
{
	float3 col = tex.Sample(smpr, uv).rgb;
	float threshold = 0.95f; //0.85以下は不自然に強調される
	float brightness = max(col.r, max(col.g, col.b));
	float compared = step(threshold, brightness);
	return col * compared;
}

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

VS_OUT VS_pass( VS_IN input )
{
	VS_OUT output;
	output.pos = float4(input.pos, 1.0f);
	output.uv = input.uv;
	return output;
}

float4 PS_pass( VS_OUT input ) : SV_TARGET
{
	return float4(GetHighBrightCol(input.uv), 1.0f);
}