#include "Precompiled.h"
#include "DxRenderTarget.h"
#include "Core/CoreInterface.h"
#include "Graphics/GraphicsInterface.h"
namespace WickedSick
{
  DxRenderTarget::DxRenderTarget(const RenderTargetDesc& desc)
  : RenderTarget(desc)
  {
  }

  DxRenderTarget::~DxRenderTarget()
  {
  }

  void DxRenderTarget::Initialize()
  {
    Graphics* gSys = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    DirectX* dx = (DirectX*) gSys->graphicsAPI;
    auto& device = dx->GetSwapChain()->device->D3DDevice;
    TextureDesc texDesc;
    texDesc.target = true;
    texDesc.size = desc_.size;
    
    texture_ = gSys->graphicsAPI->MakeTexture(texDesc);
    texture_->Initialize();
    
    D3D11_RENDER_TARGET_VIEW_DESC targetDesc;
    D3D11_TEXTURE2D_DESC dxTexDesc;
    ID3D11Texture2D* dxTex = reinterpret_cast<ID3D11Texture2D*>(texture_->GetTexturePointer());
    dxTex->GetDesc(&dxTexDesc);
    targetDesc.Format = dxTexDesc.Format;
    targetDesc.Texture2D.MipSlice = 0;
    targetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    DxError(device->CreateRenderTargetView(dxTex,
                                           &targetDesc,
                                           &render_target_));
  }

  void* DxRenderTarget::GetTargetPointer()
  {
    return render_target_;
  }
}