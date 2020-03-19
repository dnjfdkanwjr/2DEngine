#include <iostream>
#include <random>
#include "Mesh.h"
#include "DirectXDevice.h"


Vertex::Vertex() : position(0, 0, 0), normal(0, 0, 0), tangent(0, 0, 0), uv(0, 0)
{}



Vertex::Vertex(const DirectX::XMFLOAT3& p,
    const DirectX::XMFLOAT3& n,
    const DirectX::XMFLOAT3& t,
    const DirectX::XMFLOAT2& uv) : position(p), normal(n), tangent(t), uv(uv)
{}



Vertex::Vertex(FLOAT px, FLOAT py, FLOAT pz,
    FLOAT nx, FLOAT ny, FLOAT nz,
    FLOAT tx, FLOAT ty, FLOAT tz,
    FLOAT u, FLOAT v) : position(px, py, pz), normal(nx, ny, nz), tangent(tx, ty, tz), uv(u, v)
{}



Mesh::Mesh()
{

}



Mesh::~Mesh()
{

}



bool Mesh::init()
{
    vtxGPUBuffer = rp::DirectXDevice::CreateDefaultBuffer(&vertice[0], vertice.size() * sizeof(Vertex), vtxUPBuffer);
    idxGPUBuffer = rp::DirectXDevice::CreateDefaultBuffer(&indice[0], indice.size() * sizeof(UINT32),idxUPBuffer);
	RecalculateVerticeAndIndiceView();

	subMeshes.emplace_back();
	subMeshes[subMeshes.size() - 1].lastIndex = indice.size();
	subMeshes[subMeshes.size() - 1].startIndex = 0;
	subMeshes[subMeshes.size() - 1].startVertex = 0;
    return true;
}



D3D12_VERTEX_BUFFER_VIEW Mesh::GetVerticeBufferView() noexcept
{
	//if(isDirty)RecalculateVerticeAndIndiceView();


	//D3D12_VERTEX_BUFFER_VIEW vbv{};
	//vbv.BufferLocation = vtxGPUBuffer->GetGPUVirtualAddress();
	//vbv.StrideInBytes = sizeof(Vertex);
	//vbv.SizeInBytes = sizeof(Vertex) * vertice.size();

    return vbv;
}



D3D12_INDEX_BUFFER_VIEW Mesh::GetIndiceBufferView() noexcept
{
	//if (isDirty)RecalculateVerticeAndIndiceView();

	//D3D12_INDEX_BUFFER_VIEW ibv{};
	//ibv.BufferLocation = idxGPUBuffer->GetGPUVirtualAddress();
	//ibv.Format = DXGI_FORMAT_R32_UINT;
	//ibv.SizeInBytes = sizeof(UINT32) * indice.size();


    return ibv;
}

void Mesh::RecalculateVerticeAndIndiceView() noexcept
{

	vbv.BufferLocation = vtxGPUBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = sizeof(Vertex);
	vbv.SizeInBytes = sizeof(Vertex) * vertice.size();

	ibv.BufferLocation = idxGPUBuffer->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R32_UINT;
	ibv.SizeInBytes = sizeof(UINT32) * indice.size();

}

Mesh* Mesh::createGrid(FLOAT width, FLOAT depth, UINT m, UINT n, float r, float g, float b)
{
    Mesh* fornew = new Mesh();
    UINT vertexCount = m * n;
    UINT faceCount = (m - 1) * (n - 1) * 2;

    float halfWidth = 0.5f * width;
    float halfDepth = 0.5f * depth;

    float dx = width / (n - 1);
    float dz = depth / (m - 1);


    fornew->vertice.resize(vertexCount);

    for (UINT i = 0; i < m; ++i)
    {
        float z = halfDepth - i * dz;
        for (UINT j = 0; j < n; ++j)
        {
            float x = -halfWidth + j * dx;

            fornew->vertice[i * n + j].position = DirectX::XMFLOAT3(x, 0.0f, z);
            fornew->vertice[i * n + j].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
            fornew->vertice[i * n + j].tangent = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
            //fornew->vertices[i*n + j].position = DirectX::XMFLOAT3(x, 0.0f, z);
            //fornew->vertices[i*n + j].normal = DirectX::XMFLOAT3(-0.2f, -0.8f, 0.0f);
            //fornew->vertices[i*n + j].tangent = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
            // Stretch texture over grid.
            fornew->vertice[i * n + j].uv.x = j * 1.0F;
            fornew->vertice[i * n + j].uv.y = i * 1.0F;
        }
    }

    fornew->indice.resize(faceCount * 3);

    int k = 0;

    for (UINT i = 0; i < m - 1; ++i)
    {
        for (UINT j = 0; j < n - 1; ++j)
        {
            fornew->indice[k] = i * n + j;
            fornew->indice[k + 1] = i * n + j + 1;
            fornew->indice[k + 2] = (i + 1) * n + j;

            fornew->indice[k + 3] = (i + 1) * n + j;
            fornew->indice[k + 4] = i * n + j + 1;
            fornew->indice[k + 5] = (i + 1) * n + j + 1;

            k += 6; // next quad
        }
    }


    DirectX::XMFLOAT4 col = DirectX::XMFLOAT4(r, g, b, 1);

    for (size_t i = 0; i < fornew->vertice.size(); ++i)
    {
        //fornew->vtx[i] = col;
    }


    fornew->init();


    return fornew;
}


Mesh* Mesh::createGridBox(FLOAT width, FLOAT depth, FLOAT height,UINT m, UINT n, UINT l, float r, float g, float b)
{
    Mesh* fornew = new Mesh();
    UINT vertexCount = m * n* l;
    UINT faceCount = (m - 1) * (n - 1) *(l-1)* 2;

    float halfWidth = 0.5f * width;
    float halfDepth = 0.5f * height;
    float halfHeight = 0.5f * depth;

    float dx = width / (n - 1);
    float dz = depth / (m - 1);
    float dh = height / (l - 1);


    fornew->vertice.resize(vertexCount);

    for (UINT i = 0; i < m; ++i)
    {
        float z = halfDepth - i * dz;
        for (UINT j = 0; j < n; ++j)
        {
            float x = -halfWidth + j * dx;
            for (UINT k = 0; k < l; ++k) {

                float y = -halfHeight + k * dh;


                fornew->vertice[i * n + j].position = DirectX::XMFLOAT3(x, y, z);
                fornew->vertice[i * n + j].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
                fornew->vertice[i * n + j].tangent = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
                //fornew->vertices[i*n + j].position = DirectX::XMFLOAT3(x, 0.0f, z);
                //fornew->vertices[i*n + j].normal = DirectX::XMFLOAT3(-0.2f, -0.8f, 0.0f);
                //fornew->vertices[i*n + j].tangent = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
                // Stretch texture over grid.
                fornew->vertice[i * n + j].uv.x = j * 1.0F;
                fornew->vertice[i * n + j].uv.y = i * 1.0F;

            }
        }
    }

    fornew->indice.resize(faceCount * 3);

    int k = 0;

    for (UINT i = 0; i < m - 1; ++i)
    {
        for (UINT j = 0; j < n - 1; ++j)
        {
            fornew->indice[k] = i * n + j;
            fornew->indice[k + 1] = i * n + j + 1;
            fornew->indice[k + 2] = (i + 1) * n + j;

            fornew->indice[k + 3] = (i + 1) * n + j;
            fornew->indice[k + 4] = i * n + j + 1;
            fornew->indice[k + 5] = (i + 1) * n + j + 1;

            k += 6; // next quad
        }
    }


    DirectX::XMFLOAT4 col = DirectX::XMFLOAT4(r, g, b, 1);

    for (size_t i = 0; i < fornew->vertice.size(); ++i)
    {
        //fornew->vtx[i] = col;
    }


    fornew->init();


    return fornew;
}


void Mesh::MakeBoxMesh(FLOAT width, FLOAT height, FLOAT depth, Mesh* data)
{

    Vertex v[24];

    FLOAT w2 = 0.5f * width;
    FLOAT h2 = 0.5f * height;
    FLOAT d2 = 0.5f * depth;


    v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);


    v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);


    v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);



    data->vertice.assign(&v[0], &v[24]);

    UINT i[36];


    i[0] = 0; i[1] = 1; i[2] = 2;
    i[3] = 0; i[4] = 2; i[5] = 3;


    i[6] = 4; i[7] = 5; i[8] = 6;
    i[9] = 4; i[10] = 6; i[11] = 7;


    i[12] = 8; i[13] = 9; i[14] = 10;
    i[15] = 8; i[16] = 10; i[17] = 11;


    i[18] = 12; i[19] = 13; i[20] = 14;
    i[21] = 12; i[22] = 14; i[23] = 15;


    i[24] = 16; i[25] = 17; i[26] = 18;
    i[27] = 16; i[28] = 18; i[29] = 19;


    i[30] = 20; i[31] = 21; i[32] = 22;
    i[33] = 20; i[34] = 22; i[35] = 23;


    data->indice.assign(&i[0], &i[36]);

	data->init();

}
void Mesh::MakeBoxMesh2(FLOAT width, FLOAT height, FLOAT depth, Mesh* data, UINT w, UINT h, UINT d)
{


    data->vertice.reserve(w * h * d * 24);
    data->indice.reserve(w * h * d * 32);
    for (int x = 0; x < w; ++x) {

        for (int y = 0; y < h; ++y) {

            for (int z = 0; z < d; ++z) {

                int curVtxNumber = data->vertice.size();

                Vertex v[24];

                FLOAT w2 = 0.5f * width; 
                FLOAT h2 = 0.5f * height;
                FLOAT d2 = 0.5f * depth;


                float xPivot = width *x;

                float yPivot = height * y;

                float zPivot = depth * z;


                v[0] = Vertex(-w2+xPivot, -h2+yPivot, -d2+zPivot, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                v[1] = Vertex(-w2+xPivot, +h2+yPivot, -d2+zPivot, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                v[2] = Vertex(+w2+xPivot, +h2+yPivot, -d2+zPivot, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
                v[3] = Vertex(+w2+xPivot, -h2+yPivot, -d2+zPivot, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
                             
                v[4] = Vertex(-w2+xPivot, -h2 + yPivot,   +d2 + zPivot, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
                v[5] = Vertex(+w2 + xPivot, -h2 + yPivot, +d2 + zPivot, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                v[6] = Vertex(+w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                v[7] = Vertex(-w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

                v[8] = Vertex(-w2 + xPivot, +h2 + yPivot, -d2 + zPivot, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                v[9] = Vertex(-w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                v[10] = Vertex(+w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
                v[11] = Vertex(+w2 + xPivot, +h2 + yPivot, -d2 + zPivot, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

                v[12] = Vertex(-w2 + xPivot, -h2 + yPivot, -d2 + zPivot, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
                v[13] = Vertex(+w2 + xPivot, -h2 + yPivot, -d2 + zPivot, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                v[14] = Vertex(+w2 + xPivot, -h2 + yPivot, +d2 + zPivot, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                v[15] = Vertex(-w2 + xPivot, -h2 + yPivot, +d2 + zPivot, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);


                v[16] = Vertex(-w2 + xPivot, -h2 + yPivot, +d2 + zPivot, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
                v[17] = Vertex(-w2 + xPivot, +h2 + yPivot, +d2 + zPivot, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
                v[18] = Vertex(-w2 + xPivot, +h2 + yPivot, -d2 + zPivot, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
                v[19] = Vertex(-w2 + xPivot, -h2 + yPivot, -d2 + zPivot, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

                v[20] = Vertex(+w2 + xPivot, -h2 + yPivot, -d2 + zPivot, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
                v[21] = Vertex(+w2 + xPivot, +h2 + yPivot, -d2 + zPivot, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
                v[22] = Vertex(+w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
                v[23] = Vertex(+w2 + xPivot, -h2 + yPivot, +d2 + zPivot, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    
                for (int vtxI = 0; vtxI < 24; ++vtxI) {
                    data->vertice.push_back(v[vtxI]);
                }
           

                UINT i[36];
                i[0] = 0; i[1] = 1; i[2] = 2;
                i[3] = 0; i[4] = 2; i[5] = 3;


                i[6] = 4; i[7] = 5; i[8] = 6;
                i[9] = 4; i[10] = 6; i[11] = 7;


                i[12] = 8; i[13] = 9; i[14] = 10;
                i[15] = 8; i[16] = 10; i[17] = 11;


                i[18] = 12; i[19] = 13; i[20] = 14;
                i[21] = 12; i[22] = 14; i[23] = 15;


                i[24] = 16; i[25] = 17; i[26] = 18;
                i[27] = 16; i[28] = 18; i[29] = 19;


                i[30] = 20; i[31] = 21; i[32] = 22;
                i[33] = 20; i[34] = 22; i[35] = 23;


                for (int indexI = 0; indexI < 36; ++indexI) {
                    i[indexI] += curVtxNumber;
                    data->indice.push_back(i[indexI]);
                }
            }
        }

    }

    std::cout << data->vertice[0].position.x << std::endl;;
    std::cout << data->vertice[0].position.y << std::endl;;
    std::cout << data->vertice[0].position.z << std::endl;;

    std::cout << data->vertice[data->vertice.size()-1].position.x << std::endl;;
    std::cout << data->vertice[data->vertice.size() - 1].position.y << std::endl;;
    std::cout << data->vertice[data->vertice.size() - 1].position.z << std::endl;;
}

std::uniform_int_distribution<> uid(-2000, 2000);
std::default_random_engine dre;

void Mesh::MakeBoxMesh3(FLOAT width, FLOAT height, FLOAT depth, Mesh* data, UINT w, UINT h, UINT d)
{
	int seedA = uid(dre);
	int seedB = uid(dre);
	int seedC = uid(dre);


	data->vertice.reserve(w * h * d * 24);
	data->indice.reserve(w * h * d * 32);
	for (int x = 0; x < w; ++x) {

		for (int y = 0; y < h; ++y) {

			for (int z = 0; z < d; ++z) {
	
				int curVtxNumber = data->vertice.size();

				Vertex v[24];

				FLOAT w2 = 0.5f * width;
				FLOAT h2 = 0.5f * height;
				FLOAT d2 = 0.5f * depth;

				float ns = 1;

				float xPivot = width  * x* ns;  
				float yPivot = height * y* ns;
				float zPivot = depth  * z* ns;




				v[0] = Vertex(-w2 + xPivot, -h2 + yPivot, -d2 + zPivot, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				v[1] = Vertex(-w2 + xPivot, +h2 + yPivot, -d2 + zPivot, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				v[2] = Vertex(+w2 + xPivot, +h2 + yPivot, -d2 + zPivot, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
				v[3] = Vertex(+w2 + xPivot, -h2 + yPivot, -d2 + zPivot, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

				v[4] = Vertex(-w2 + xPivot, -h2 + yPivot, +d2 + zPivot, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
				v[5] = Vertex(+w2 + xPivot, -h2 + yPivot, +d2 + zPivot, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				v[6] = Vertex(+w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				v[7] = Vertex(-w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

				v[8] = Vertex(-w2 + xPivot, +h2 + yPivot, -d2 + zPivot, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				v[9] = Vertex(-w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				v[10] = Vertex(+w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
				v[11] = Vertex(+w2 + xPivot, +h2 + yPivot, -d2 + zPivot, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

				v[12] = Vertex(-w2 + xPivot, -h2 + yPivot, -d2 + zPivot, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
				v[13] = Vertex(+w2 + xPivot, -h2 + yPivot, -d2 + zPivot, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				v[14] = Vertex(+w2 + xPivot, -h2 + yPivot, +d2 + zPivot, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				v[15] = Vertex(-w2 + xPivot, -h2 + yPivot, +d2 + zPivot, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);


				v[16] = Vertex(-w2 + xPivot, -h2 + yPivot, +d2 + zPivot, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
				v[17] = Vertex(-w2 + xPivot, +h2 + yPivot, +d2 + zPivot, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
				v[18] = Vertex(-w2 + xPivot, +h2 + yPivot, -d2 + zPivot, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
				v[19] = Vertex(-w2 + xPivot, -h2 + yPivot, -d2 + zPivot, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

				v[20] = Vertex(+w2 + xPivot, -h2 + yPivot, -d2 + zPivot, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
				v[21] = Vertex(+w2 + xPivot, +h2 + yPivot, -d2 + zPivot, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
				v[22] = Vertex(+w2 + xPivot, +h2 + yPivot, +d2 + zPivot, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
				v[23] = Vertex(+w2 + xPivot, -h2 + yPivot, +d2 + zPivot, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);


				for (int vtxI = 0; vtxI < 24; ++vtxI) {
					data->vertice.push_back(v[vtxI]);
				}


				UINT i[36];
				i[0] = 0; i[1] = 1; i[2] = 2;
				i[3] = 0; i[4] = 2; i[5] = 3;


				i[6] = 4; i[7] = 5; i[8] = 6;
				i[9] = 4; i[10] = 6; i[11] = 7;


				i[12] = 8; i[13] = 9; i[14] = 10;
				i[15] = 8; i[16] = 10; i[17] = 11;


				i[18] = 12; i[19] = 13; i[20] = 14;
				i[21] = 12; i[22] = 14; i[23] = 15;


				i[24] = 16; i[25] = 17; i[26] = 18;
				i[27] = 16; i[28] = 18; i[29] = 19;


				i[30] = 20; i[31] = 21; i[32] = 22;
				i[33] = 20; i[34] = 22; i[35] = 23;


				for (int indexI = 0; indexI < 36; ++indexI) {
					i[indexI] += curVtxNumber;
					data->indice.push_back(i[indexI]);
				}
			}
		}

	}

	std::cout << data->vertice[0].position.x << std::endl;;
	std::cout << data->vertice[0].position.y << std::endl;;
	std::cout << data->vertice[0].position.z << std::endl;;

	std::cout << data->vertice[data->vertice.size() - 1].position.x << std::endl;;
	std::cout << data->vertice[data->vertice.size() - 1].position.y << std::endl;;
	std::cout << data->vertice[data->vertice.size() - 1].position.z << std::endl;;
}


void MakeQuadMesh(Mesh* data)
{
    data->vertice.resize(4);
    data->indice.resize(6);

    data->vertice[0] = Vertex(
        -1.0f, -1.0f, 0.0f,    //3Â÷¿ø ÁÂÇ¥
        0.0f, 0.0f, -1.0f,    //¹ý¼± º¤ÅÍ
        1.0f, 0.0f, 0.0f,    //Á¢¼± º¤ÅÍ
        0.0f, 1.0f);          //ÅØ½ºÃÄ ÁÂÇ¥

    data->vertice[1] = Vertex(-1.0f, +1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    data->vertice[2] = Vertex(+1.0f, +1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    data->vertice[3] = Vertex(+1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    data->indice[0] = 0;
    data->indice[1] = 1;
    data->indice[2] = 2;

    data->indice[3] = 0;
    data->indice[4] = 2;
    data->indice[5] = 3;
}