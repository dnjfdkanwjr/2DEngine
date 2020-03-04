#include "Window.h"
#include "Graphics.h"

//std::unique_ptr<Window> GWindow {}; 
HWND                        Window::hwnd    { };
WNDCLASSEX                  Window::wc      { };
std::wstring_view           Window::title   { L"MapTool" };



void Window::initialize()
{
    wc = { sizeof(WNDCLASSEX), CS_CLASSDC, Window::wndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    
    ::RegisterClassEx(&wc);

    hwnd = ::CreateWindow(wc.lpszClassName, title.data(), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    Graphics::initialize();
}

void Window::run()
{
    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(Graphics::getDevice(), Graphics::getDeviceContext());
    ImGui::StyleColorsDark();

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Map", 0, ImGuiWindowFlags_NoMove);
            enum Mode
            {
                Mode_Copy,
                Mode_Move,
                Mode_Swap
            };
            static int mode = 0;
            if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
            if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
            if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }
            static const char* names[9] = { "Bobby", "Beatrice", "Betty", "Brianna", "Barry", "Bernard", "Bibi", "Blaine", "Bryn" };
            for (int n = 0; n < IM_ARRAYSIZE(names); n++)
            {
                ImGui::PushID(n);
                if ((n % 3) != 0)
                    ImGui::SameLine();
                ImGui::Button(names[n], ImVec2(60, 60));

                // Our buttons are both drag sources and drag targets here!
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));    // Set payload to carry the index of our item (could be anything)
                    if (mode == Mode_Copy) { ImGui::Text("Copy %s", names[n]); }    // Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
                    if (mode == Mode_Move) { ImGui::Text("Move %s", names[n]); }
                    if (mode == Mode_Swap) { ImGui::Text("Swap %s", names[n]); }
                    ImGui::EndDragDropSource();
                }

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int payload_n = *(const int*)payload->Data;
                        if (mode == Mode_Copy)
                        {
                            names[n] = names[payload_n];
                        }
                        if (mode == Mode_Move)
                        {
                            names[n] = names[payload_n];
                            names[payload_n] = "";
                        }
                        if (mode == Mode_Swap)
                        {
                            const char* tmp = names[n];
                            names[n] = names[payload_n];
                            names[payload_n] = tmp;
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::PopID();
            }

            ImGui::TreePop();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();

        }

        {
            ImGui::Begin("IO", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
            static const ImVec2 saveButton(100, 100);
            if (ImGui::Button("Save", saveButton))
            {
            }

            ImGui::SameLine();
            if (ImGui::Button("Open", saveButton))
            {
            }
            ImGui::End();
        }

        {
            ImGui::Begin("Structure", 0, /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoCollapse);
            ImGui::SetNextItemWidth(300.00f);
            static int currentTileWidth[2] = { 0 , 0 };

            ImTextureID my_tex_id = Graphics::getIO().Fonts->TexID;
            float my_tex_w = (float)Graphics::getIO().Fonts->TexWidth;
            float my_tex_h = (float)Graphics::getIO().Fonts->TexHeight;

            ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
            ImGui::SliderInt("v", &currentTileWidth[0], 0, 150);            // Edit 1 float using a slider
            ImGui::End();
        }

        {
            ImGui::Begin("Tile", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
            static int currentTileWidth[2] = { 0 , 0};
            ImGui::SetNextItemWidth(300.00f);

            ImGui::SliderInt("v", &currentTileWidth[0], 0, 150);            // Edit 1 float using a slider
            ImGui::SliderInt2("v", currentTileWidth, 0, 150);            // Edit 1 float using a slider


            ImGui::End();
        }

        Graphics::render();
        Graphics::present();
    }
}

void Window::finalize()
{
    Graphics::finalize();

    ::DestroyWindow(hwnd);

    ::UnregisterClass(getWcEx().lpszClassName, getWcEx().hInstance);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall Window::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
        case WM_SIZE:
            if (Graphics::getDevice() != NULL && wParam != SIZE_MINIMIZED)
            {
                Graphics::cleanupRenderTarget();
                Graphics::getSwapChain()->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD (lParam), DXGI_FORMAT_UNKNOWN, 0);
                Graphics::createRenderTarget();
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

std::wstring_view& Window::getTitle()
{
    return title;
}

HWND& Window::getHwnd()
{
    return hwnd;
}

WNDCLASSEX& Window::getWcEx()
{
    return wc;
}
