project('engine', 'cpp',
    default_options : ['cpp_std=c++20'])

# Files
python = find_program(['python.exe', 'python'])
glob = run_command('python', 'file_glob.py', check : true)
src_files = files(glob.stdout().split(' '))
inc_dirs = include_directories('includes')

executable('window',
    sources : src_files,
    include_directories : inc_dirs,
    gui_app : true)