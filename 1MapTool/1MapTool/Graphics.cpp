#include "Graphics.h"
#include "Window.h"


ID3D11Device*				Graphics::device                { nullptr };
ID3D11DeviceContext*		Graphics::device_context        { nullptr };
IDXGISwapChain*				Graphics::swap_chain            { nullptr };
ID3D11RenderTargetView*		Graphics::render_target_view    { nullptr };

ImVec4                      Graphics::bg_color{};
ImFont*                     Graphics::font{};
ImGuiIO                     Graphics::imgui_io{};


bool Graphics::initialize()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    imgui_io = ImGui::GetIO(); //(void)io;
    imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    if (!createDeviceD(Window::getHwnd()))
    {
        cleanupDeviceD();
        ::UnregisterClass(Window::getWcEx().lpszClassName, Window::getWcEx().hInstance);
        
        return false;
    }

    // Load Fonts
    Graphics::loadFont("");

    return true;
}

void Graphics::finalize()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    Graphics::cleanupDeviceD();
   
}

void Graphics::render()
{
    ImGui::Render();

    Graphics::device_context->OMSetRenderTargets(1, &Graphics::render_target_view, NULL);
    Graphics::device_context->ClearRenderTargetView(Graphics::render_target_view, (float*)&bg_color);
    
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void Graphics::present()
{
    Graphics::swap_chain->Present(1, 0); 
}


bool Graphics::createDeviceD(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &swap_chain, &device, &featureLevel, &device_context) != S_OK)
        return false;

    createRenderTarget();
    return true;
}

void Graphics::cleanupDeviceD()
{
    cleanupRenderTarget();
    if (swap_chain) 
    { 
        swap_chain->Release(); 
        swap_chain = NULL; 
    }

    if (device_context) 
    { 
        device_context->Release(); 
        device_context = NULL; 
    }

    if (device) 
    { 
        device->Release(); 
        device = NULL; 
    }
}

void Graphics::createRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    device->CreateRenderTargetView(pBackBuffer, NULL, &render_target_view);
    pBackBuffer->Release();
}

void Graphics::cleanupRenderTarget()
{
    if (render_target_view) 
    { 
        render_target_view->Release(); 
        render_target_view = NULL; 
    }
}

void Graphics::loadFont(const std::string_view& font_path)
{
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !


    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

}

ID3D11Device* Graphics::getDevice()
{
    return Graphics::device;
}

ID3D11DeviceContext* Graphics::getDeviceContext()
{
    return Graphics::device_context;
}

IDXGISwapChain* Graphics::getSwapChain()
{
    return swap_chain;
}

ID3D11RenderTargetView* Graphics::getRenderTargetView()
{
    return render_target_view;
}

ImGuiIO& Graphics::getIO()
{
    return imgui_io;
}
