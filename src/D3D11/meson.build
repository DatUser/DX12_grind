d3d11_src = [
    'D3D11Buffer.cpp',
    'D3D11Common.cpp',
    'D3D11Shader.cpp',
    'D3D11Texture.cpp',
    'D3D11Viewport.cpp',
    'D3D11Commands.cpp',
    'D3D11Interface.cpp',
    'D3D11Swapchain.cpp'
]

d3d11_inc = include_directories('../../../includes/D3D11')

d3d11_lib = static_library('D3D11',
                      sources: d3d11_src,
                      include_directories: [includes_dir, d3d11_inc],
                      dependencies: [rhi_dep]  # Explicit dependency on core
)

d3d11_dep = declare_dependency(
    link_with: d3d11_lib,
    include_directories: [includes_dir, d3d11_inc]
)