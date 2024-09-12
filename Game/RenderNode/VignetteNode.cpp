#include "VignetteNode.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

void VignetteNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	vignetteBuffer.get_data()->color = { 1.0f,0.0f,0.0f,0.5f };
	vignetteBuffer.get_data()->level = 1.0f;
	vignetteBuffer.get_data()->gradient = 20.0f;
}

void VignetteNode::draw() {
	auto&& command = DirectXCommand::GetCommandList();
	command->SetGraphicsRootDescriptorTable(1, textureGPUHandle);
	command->SetGraphicsRootConstantBufferView(0, vignetteBuffer.get_resource()->GetGPUVirtualAddress());
	command->DrawInstanced(3, 1, 0, 0);
}

void VignetteNode::set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	textureGPUHandle = textureGPUHandle_;
}

void VignetteNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0
	);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"Engine/HLSL/FullscreenShader.hlsl",
		"Game/RenderNode/Vignette.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());

}

void VignetteNode::set_level(float level_) {
	vignetteBuffer.get_data()->level = level_;
}

void VignetteNode::set_gradient(float gradient) {
	vignetteBuffer.get_data()->gradient = gradient;
}

void VignetteNode::set_color(Color color) {
	vignetteBuffer.get_data()->color = color;
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void VignetteNode::debug_gui() {
	if (ImGui::CollapsingHeader("VignetteNode", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::ColorEdit4("Color", &vignetteBuffer.get_data()->color.red);
		ImGui::DragFloat("Level", &vignetteBuffer.get_data()->level, 0.005f, 0.0f, 1.0f);
		ImGui::DragFloat("Gradient", &vignetteBuffer.get_data()->gradient, 0.5f);
	}
}
#endif // _DEBUG
