engine_src = [
    'app.cpp',
    'camera.cpp',
    'mesh.cpp',
    'renderer.cpp',
    'scene.cpp',
    'transform.cpp',
    'window.cpp'
]

engine_inc = include_directories('../../../includes/Engine')

engine_lib = static_library('Engine',
                      sources: engine_src,
                      include_directories: [includes_dir, engine_inc],
                      dependencies: [rhi_dep, io_dep]  # Explicit dependency on core
)

engine_dep = declare_dependency(
    link_with: engine_lib,
    include_directories: [includes_dir, engine_inc]
)