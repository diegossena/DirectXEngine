//
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
//
#include "Engine/Graphics/Color.h"
#include "Engine/Graphics/Vertex.h"
#include "Engine/Graphics/Position.h"
//
#include "Engine/Window.h"
//
class Graphics
{
private:
  // Props
  IDXGISwapChain *swapchain;
  ID3D11Device *dev;
  ID3D11DeviceContext *devcon;
  ID3D11RenderTargetView *backbuffer;

  ID3D11InputLayout *layout;        // the pointer to the input layout
  ID3D11VertexShader *vortexShader; // the pointer to the vertex shader
  ID3D11PixelShader *pixelShader;   // the pointer to the pixel shader
  ID3D11Buffer *vertexBuffer;       // the pointer to the vertex buffer
  ID3D11Buffer *indexBuffer;        // the pointer to the index buffer
  // Create Buffer
  D3D11_BUFFER_DESC bufferDesc = {
      0,                      // ByteWidth
      D3D11_USAGE_DYNAMIC,    // USAGE: write access access by CPU and GPU
      0,                      // BindFlags
      D3D11_CPU_ACCESS_WRITE, // allow CPU to write in buffer
      0,                      // MiscFlags
      0                       //StructureByteStride
  };
  D3D11_SUBRESOURCE_DATA subResourceData;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  // Methods
  void InitPipeline()
  {
    // Vortex Shader
    ID3DBlob *vsBlob;
    D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vsBlob, 0);
    dev->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &vortexShader);
    devcon->VSSetShader(vortexShader, 0, 0);
    // Pixel SHader
    ID3DBlob *psBlob;
    D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &psBlob, 0);
    dev->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &pixelShader);
    devcon->PSSetShader(pixelShader, 0, 0);
    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}};

    dev->CreateInputLayout(ied, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &layout);
    devcon->IASetInputLayout(layout);
  }
  void InitGraphics()
  {
    // select which primtive type we are using
    devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  }

public:
  Graphics(const Window &window)
  {
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
    scd.BufferDesc.Width = window.screen.width;         // set the back buffer width
    scd.BufferDesc.Height = window.screen.height;       // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how swap chain is to be used
    scd.OutputWindow = window.hWnd;                     // the window to be used
    scd.SampleDesc.Count = 4;                           // how many multisamples
    scd.Windowed = TRUE;                                // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(0,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  0,
                                  0,
                                  0,
                                  0,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &dev,
                                  0,
                                  &devcon);

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, 0, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, 0);

    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = window.screen.width;
    viewport.Height = window.screen.height;

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
  }
  ~Graphics()
  {
    // close and release all existing COM objects
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
    layout->Release();
    vortexShader->Release();
    pixelShader->Release();
    vertexBuffer->Release();
    indexBuffer->Release();
  }
  void backgroundColor(fRGBA &color)
  {
    devcon->ClearRenderTargetView(backbuffer, (float *)&color);
  }
  void backgroundColor(fRGBA color)
  {
    devcon->ClearRenderTargetView(backbuffer, (float *)&color);
  }
  void clear()
  {
    vertices.clear();
    indices.clear();
  }
  void render(void)
  {
    // Create Vertex Buffer
    bufferDesc.ByteWidth = vertices.size() * sizeof(Vertex); // size is the VERTEX struct
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;         // use as a vertex buffer
    subResourceData = {vertices.data(), 0, 0};
    dev->CreateBuffer(&bufferDesc, &subResourceData, &vertexBuffer); // create the buffer
    // Create Index Buffer
    bufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subResourceData = {indices.data(), 0, 0};
    dev->CreateBuffer(&bufferDesc, &subResourceData, &indexBuffer);
    /* select which vertex buffer to display
    -----------------------------------------*/
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    vertexBuffer->Release();
    devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    indexBuffer->Release();
    // draw the vertex buffer to the back buffer
    devcon->DrawIndexed(indices.size(), 0, 0);
    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
  }
  /* Shapes
  ----------*/
  void triangle(
      Vertex &vertice0,
      Vertex &vertice1,
      Vertex &vertice2)
  {
    unsigned short count = vertices.size();
    Graphics::indices.push_back(count);
    count++;
    Graphics::indices.push_back(count);
    count++;
    Graphics::indices.push_back(count);
    Graphics::vertices.push_back(vertice0);
    Graphics::vertices.push_back(vertice1);
    Graphics::vertices.push_back(vertice2);
  }
  void rectangle(
      const Vertex &vertice0,
      const Vertex &vertice1,
      const Vertex &vertice3,
      const Vertex &vertice2)
  {
    unsigned short vertI0 = vertices.size();
    unsigned short vertI1 = vertI0 + 1;
    unsigned short vertI2 = vertI0 + 2;
    unsigned short vertI3 = vertI0 + 3;
    // Triangle
    Graphics::indices.push_back(vertI0);
    Graphics::indices.push_back(vertI1);
    Graphics::indices.push_back(vertI2);
    // Triangle
    Graphics::indices.push_back(vertI1);
    Graphics::indices.push_back(vertI3);
    Graphics::indices.push_back(vertI2);
    // Vertices
    Graphics::vertices.push_back(vertice0);
    Graphics::vertices.push_back(vertice1);
    Graphics::vertices.push_back(vertice2);
    Graphics::vertices.push_back(vertice3);
  }
  void rectangle(const float &x1, const float &x2, const float &y1, const float &y2, const RGBA &color)
  {
    rectangle(
        {Position3D(x1, y1, 0.0f), color},
        {Position3D(x2, y1, 0.0f), color},
        {Position3D(x2, y2, 0.0f), color},
        {Position3D(x1, y2, 0.0f), color});
  }
  void square(Position2D &position, Position2D &distance, const RGBA &color)
  {
    rectangle(position.x, position.x + distance.x, position.y, position.y - distance.y, color);
  }
};