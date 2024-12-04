project('engine', 'cpp',
    default_options : ['cpp_std=c++20'])

# Files
python = find_program(['python.exe', 'python'])
glob = run_command('python', 'file_glob.py', check : true)
src_files = files(glob.stdout().split(' '))

# Download tiny_obj_loader.h
tiny_obj_loader = run_command(
	'cmd' , '/C', 'mkdir', 'includes\\external\\', '&&',
    'curl', '-L', '-o', 'includes/external/tiny_obj_loader.h',
    'https://github.com/tinyobjloader/tinyobjloader/releases/download/v1.0.6/tiny_obj_loader.h'
)

inc_dirs = include_directories('includes')

executable('window',
    sources : src_files,
    include_directories : inc_dirs,
	#dependencies: [tiny_obj_loader_dep],
    gui_app : true)