import glob

source_files = glob.glob("src/**/*.cpp", recursive=True)
out = ''
for filename in source_files[:-1]:
    out += filename + ' '
out +=  source_files[-1]

print(out, end='')