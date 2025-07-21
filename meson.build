project('engine', 'cpp',
        default_options: [
            'cpp_std=c++20',
            'b_vscrt=static_from_buildtype',
            'default_library=static'  # If using static libs
        ]
)

# Ensure external includes folder
fs = import('fs')
if not fs.exists('includes\\external\\')
    run_command('cmd' , '/C', 'mkdir','includes\\external\\', check : true)
endif

# Download tiny_obj_loader.h
tiny_obj_loader = run_command(
    'curl', '-L', '-o', 'includes/external/tiny_obj_loader.h',
    'https://github.com/tinyobjloader/tinyobjloader/releases/download/v1.0.6/tiny_obj_loader.h',
    check : true
)

# Download stb_image.h
stb_image = run_command(
    'curl', '-L', '-o', 'includes/external/stb_image.h',
    'https://raw.githubusercontent.com/nothings/stb/master/stb_image.h',
    check : true
)

# Include directories
includes_dir = include_directories('includes')

# Include source subdirectories
subdir('src')

# Main executable
executable('window',
           'src/main.cpp',  # If you have a main.cpp
           link_with: [rhi_lib, io_lib, d3d11_lib, engine_lib, input_lib],
           include_directories: includes_dir,
           #dependencies: [external_deps],
           install: true,
           gui_app: true
)