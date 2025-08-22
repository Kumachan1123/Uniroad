/*
    @file	DepthStencil.h
    @brief	深度ステンシル作成用ヘルパークラス、DX::RenderTextureクラスを参考に作成
*/
#pragma once

namespace mylib
{
    class DepthStencil
    {
    public:
        explicit DepthStencil(DXGI_FORMAT format) noexcept;

        DepthStencil(DepthStencil&&) = default;
        DepthStencil& operator= (DepthStencil&&) = default;

        DepthStencil(DepthStencil const&) = delete;
        DepthStencil& operator= (DepthStencil const&) = delete;

        void SetDevice(_In_ ID3D11Device* device);

        void SizeResources(size_t width, size_t height);

        void ReleaseDevice() noexcept;

        void SetWindow(const RECT& rect);

        ID3D11Texture2D* GetTexture2D() const noexcept { return m_texture2D.Get(); }
        ID3D11DepthStencilView* GetDepthStencilView() const noexcept { return m_depthStencilView.Get(); }

        DXGI_FORMAT GetFormat() const noexcept { return m_format; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Device>                m_device;
        Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_texture2D;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_depthStencilView;

        DXGI_FORMAT                                         m_format;

        size_t                                              m_width;
        size_t                                              m_height;
    };
}
