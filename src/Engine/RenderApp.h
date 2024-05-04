#pragma once

#include "BaseApp.h"

constexpr int SwapChainBufferCount = 2;

struct RenderSystemCreateInfo final
{

};

class RenderApp : public BaseApp
{
public:
	virtual void CreateRtvAndDsvDescriptorHeaps();
	void OnWindowSizeChanged() override;

protected:
	RenderApp() = default;
	virtual ~RenderApp() = default;

	bool initRenderApp(const RenderSystemCreateInfo& createInfo);
	void closeRenderApp();

//private: // временно открыто
	void logAdapters();
	void logAdapterOutputs(IDXGIAdapter* adapter);
	void logOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
	void createCommandObjects();
	void createSwapChain();
	void flushCommandQueue();

	ID3D12Resource* currentBackBuffer() const;
	D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView() const;

	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;

	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
	UINT64 m_currentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_directCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

	int m_currBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_swapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;

	D3D12_VIEWPORT m_screenViewport;
	D3D12_RECT m_scissorRect;

	UINT m_rtvDescriptorSize = 0;
	UINT m_dsvDescriptorSize = 0;
	UINT m_cbvSrvUavDescriptorSize = 0;

	DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	bool m_4xMsaaState = false; // 4X MSAA enabled
	UINT m_4xMsaaQuality = 0; // quality level of 4X MSAA
};