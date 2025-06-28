io_src = [
    'objloader.cpp'
]

io_inc = include_directories('../../../includes/IO')

io_lib = static_library('IO',
                      sources: io_src,
                      include_directories: [includes_dir, io_inc],
                      dependencies: [rhi_dep]  # Explicit dependency on core
)

io_dep = declare_dependency(
    link_with: io_lib,
    include_directories: [includes_dir, io_inc]
)