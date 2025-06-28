input_src = [
    'controller.cpp',
    'keyboard.cpp',
    'mouse.cpp'
]

input_inc = include_directories('../../../includes/Input')

input_lib = static_library('Input',
                      sources: input_src,
                      include_directories: [includes_dir, input_inc],
                      dependencies: [engine_dep]  # Explicit dependency on core
)

input_dep = declare_dependency(
    link_with: input_lib,
    include_directories: [includes_dir, input_inc]
)