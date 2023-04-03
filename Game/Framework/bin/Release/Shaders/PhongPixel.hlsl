struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 posw : POSITION0;
	float4 color : COLOR;
	float3 norw : NORMAL0;
	float2 tex : TEXCOORD0;
};

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

struct DirectionalLight
{
	float4 dir;       //(x,y,z,0)
	float4 diffuse;   //(r,g,b,0)
	float4 specColor; //(r,g,b,0)
};

struct Material
{
	float4 diffuse;  //(r,g,b,0)
	float4 specular; //(r,g,b,specPow)
	float4 ambient;  //(r,g,b,0)
	float4 emissive; //(r,g,b,a)
};

cbuffer ConstantBuffer 
{
	float4 eyePos;  //(x,y,z,0)
	float4 ambient; //(x,y,z,0)
	DirectionalLight dirLight;
	Material mat;
}

float4 main(PS_IN input) : SV_Target
{
	float3 L = normalize(-dirLight.dir.xyz);
	float3 N = normalize(input.norw);
	float3 V = normalize(eyePos.xyz - input.posw.xyz);
	float3 R = N * dot(N,L) * 2 - L;
	float plusVal = 0.2f;

	float3 amb = saturate(mat.ambient.xyz * ambient.xyz);
	float3 dif = saturate(dot(L, N)) * mat.diffuse.xyz * dirLight.diffuse.xyz;
	float3 spec = saturate(pow(saturate(dot(R, V)), mat.specular.w) * mat.specular.xyz * dirLight.specColor.xyz);
	float4 col = (float4(amb + dif + spec, 1.0)* myTexture.Sample(mySampler, input.tex))
				  + (input.color * mat.emissive);

	return col;
}