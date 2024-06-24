#pragma once

#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"
#include "Engine/WinApp.h"

class VertexBuffer;

class OffscreenRender final : public RenderTarget {
public:
	OffscreenRender() noexcept;
	~OffscreenRender() noexcept;

	OffscreenRender(const OffscreenRender&) = delete;
	OffscreenRender& operator=(const OffscreenRender&) = delete;
	OffscreenRender(OffscreenRender&&) = default;
	OffscreenRender& operator=(OffscreenRender&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void initialize(UINT64 width = WinApp::GetClientWidth() , UINT height = WinApp::GetClientHight());

	/// <summary>
	/// リソースバリアの状態遷移
	/// </summary>
	void change_resource_state() override;

	/// <summary>
	/// SRVGPUHandleの取得
	/// </summary>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE texture_gpu_handle() const;

	/// <summary>
	/// Viewに使用しているインデックスを返す
	/// </summary>
	void release_index() const override;

	/// <summary>
	/// 明示delete(外部から使用しない)
	/// </summary>
	void create_view() = delete;

private:
	/// <summary>
	/// リソース生成
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void create_resource(UINT64 width, UINT height);

	/// <summary>
	/// SRVの作成
	/// </summary>
	void create_textue_view();

private:
	std::optional<std::uint32_t> srvHeapIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE srvCPUHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUHandle{};
};

