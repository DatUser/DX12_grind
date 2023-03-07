project('engine', 'cpp')

python = find_program(['python.exe', 'python'])
glob = run_command('python', 'file_glob.py', check : true)
src_files = files(glob.stdout().split(' '))

executable('window', sources : src_files)