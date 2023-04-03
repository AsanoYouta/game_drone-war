Texture2D tex1 : register(t0); 
Texture2D tex2 : register(t1); 
SamplerState smpr : register(s0);

cbuffer ConstantBuffer 
{
	float2 viewPort; //(width, height)
	float2 options; //(offset, intensity)
	float gausWeight[16];
}

float2 GetTexelSize(float width, float height)
{
	float deltax = 1.0f / width;
	float deltay = 1.0f / height;
	return float2(deltax, deltay);
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

float4 PS_pass1(VS_OUT input) : SV_TARGET
{
	float4 col = 0;
	float2 d = GetTexelSize(viewPort.x, viewPort.y);
	input.uv -= d/2;  //uvを左上境界上に移動

	[unroll]
	for(int i=0; i < 16; i++)
	{
		//左右の重み付きテクセル取得
		col += (tex1.Sample(smpr, input.uv + float2(d.x, 0)*(i*options.x)) 
			 + tex1.Sample(smpr, input.uv - float2(d.x, 0)*(i*options.x))) * gausWeight[i];
	}
	col *= 2;
	//輝度強調
	col *= options.y;

	return col*2.0f;
}

float4 PS_pass2(VS_OUT input) : SV_TARGET
{
	float4 gCol = 0;
	float2 d = GetTexelSize(viewPort.x, viewPort.y);
	input.uv -= d/2;  //uvを左上境界上に移動

	[unroll]
	for(int i=0; i < 16; i++)
	{
		//上下の重み付きテクセル取得
		gCol += (tex2.Sample(smpr, input.uv + float2(0, d.y)*(i*options.x)) 
			 + tex2.Sample(smpr, input.uv - float2(0, d.y)*(i*options.x))) * gausWeight[i];
	}
	gCol *= 2;
	//輝度強調
	//gCol *= options.y;

	return gCol + tex1.Sample(smpr,input.uv);
}