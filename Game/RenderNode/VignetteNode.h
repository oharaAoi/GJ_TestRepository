#pragma once

#include <Engine/Render/RenderNode/SingleRenderTargetNode.h>

#include <Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h>
#include <Engine/Game/Color/Color.h>

class VignetteNode : public SingleRenderTargetNode {
public:
	VignetteNode() = default;
	~VignetteNode() = default;

	VignetteNode(const VignetteNode&) = delete;
	VignetteNode& operator=(const VignetteNode&) = delete;
	VignetteNode(VignetteNode&&) = default;
	VignetteNode& operator=(VignetteNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	/// <summary>
	/// 描画
	/// </summary>
	void draw();

	/// <summary>
	/// 描画時に使用するテクスチャリソースを設定
	/// </summary>
	/// <param name="textureGPUHandle_">テクスチャのSRVGPUハンドル</param>
	void set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_);

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

public:
	void set_level(float level);
	void set_gradient(float gradient);
	void set_color(Color color);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // DEBUG

private:
	D3D12_GPU_DESCRIPTOR_HANDLE textureGPUHandle{};

	struct VignetteBuffer {
		Color color;
		float level;
		float gradient;
	};
	ConstantBuffer<VignetteBuffer> vignetteBuffer;
};

