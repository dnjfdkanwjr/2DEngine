#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl\client.h>

struct Vertex {

    DirectX::XMFLOAT3 position;  
    DirectX::XMFLOAT3 normal;    
    DirectX::XMFLOAT3 tangent;	
    DirectX::XMFLOAT2 uv;		


    Vertex();
    Vertex(const DirectX::XMFLOAT3& p,
        const DirectX::XMFLOAT3& n,
        const DirectX::XMFLOAT3& t,
        const DirectX::XMFLOAT2& uv);


    Vertex(FLOAT px, FLOAT py, FLOAT pz,
        FLOAT nx, FLOAT ny, FLOAT nz,
        FLOAT tx, FLOAT ty, FLOAT tz,
        FLOAT u, FLOAT v);
};

struct Mesh
{
	Microsoft::WRL::ComPtr<ID3D12Resource> vtxGPUBuffer{};
	Microsoft::WRL::ComPtr<ID3D12Resource> idxGPUBuffer{};

	Microsoft::WRL::ComPtr<ID3D12Resource> vtxUPBuffer{};
	Microsoft::WRL::ComPtr<ID3D12Resource> idxUPBuffer{};

	std::vector<Vertex>  vertice{};
	std::vector<UINT32>  indice{};


	bool isDirty{};
	D3D12_VERTEX_BUFFER_VIEW vbv{};
	D3D12_INDEX_BUFFER_VIEW ibv{};

    Mesh();
    virtual ~Mesh();

    bool init();
    D3D12_VERTEX_BUFFER_VIEW& GetVerticeBufferView()noexcept;
    D3D12_INDEX_BUFFER_VIEW&  GetIndiceBufferView()noexcept;
	void RecalculateVerticeAndIndiceView()  noexcept;

    static void MakeBoxMesh(FLOAT width, FLOAT height, FLOAT depth, Mesh* data);
    static void MakeBoxMesh2(FLOAT width, FLOAT height, FLOAT depth, Mesh* data, UINT w, UINT h, UINT d);
	static void MakeBoxMesh3(FLOAT width, FLOAT height, FLOAT depth, Mesh* data, UINT w, UINT h, UINT d);
    static Mesh* createGrid(FLOAT width, FLOAT depth, UINT m, UINT n, float r, float g, float b);
    static Mesh* createGridBox(FLOAT width, FLOAT depth, FLOAT height, UINT m, UINT n, UINT l, float r, float g, float b);
};

