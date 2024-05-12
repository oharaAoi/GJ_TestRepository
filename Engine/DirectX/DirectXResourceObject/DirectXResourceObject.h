#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

class DirectXResourceObject {
public:
	DirectXResourceObject() = default;
	virtual ~DirectXResourceObject() = default;

public:
	// コピーは禁止
	DirectXResourceObject(const DirectXResourceObject&) = delete;
	DirectXResourceObject& operator=(const DirectXResourceObject&) = delete;
	// ムーブは許可
	DirectXResourceObject(DirectXResourceObject&& rhs) noexcept;
	DirectXResourceObject& operator=(DirectXResourceObject&& rhs) noexcept;

public:
	Microsoft::WRL::ComPtr<ID3D12Resource>& get_resource();
	const Microsoft::WRL::ComPtr<ID3D12Resource>& get_resource() const;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

protected:
	[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(std::uint32_t sizeInByte);
};