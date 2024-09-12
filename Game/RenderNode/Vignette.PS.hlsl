struct VertexShaderOutput {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

struct VignetteBuffer {
	float4 color; // 色
	float level;
	float gradient;
};

ConstantBuffer<VignetteBuffer> gVignette : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

float4 main(VertexShaderOutput input) : SV_TARGET0 {
	float4 outputColor;
	float4 sampleColor = gTexture.Sample(gSampler, input.texcoord);
	float2 current = input.texcoord * (1.0f - input.texcoord.yx);
	float vignette = saturate(pow(current.x * current.y * gVignette.gradient, gVignette.level));
	float alpha = (1 - vignette) * gVignette.color.a;
	outputColor.rgb = sampleColor.rgb * (1 - alpha) + (gVignette.color.rgb * alpha);
	outputColor.a = sampleColor.a;
	return outputColor;
}