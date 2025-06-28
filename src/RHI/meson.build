rhi_src = [
    'rhi.cpp',
    'rhi_buffer.cpp',
    'rhi_shader.cpp',
    'rhi_viewport.cpp'
]

rhi_inc = include_directories('../../../includes/RHI')

rhi_lib = static_library('RHI',
                      sources: rhi_src,
                      include_directories: [includes_dir, rhi_inc]
)

rhi_dep = declare_dependency(
    link_with: rhi_lib,
)