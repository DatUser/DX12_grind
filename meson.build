project('engine', 'cpp')

python = find_program(['python.exe', 'python'])
glob = run_command('python', 'file_glob.py')
src_files = files(glob)

executable('window.exe', sources : src_files)