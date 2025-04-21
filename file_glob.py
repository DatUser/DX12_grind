import glob

source_files = []
source_files += glob.glob("src/RHI/*.cpp", recursive=False)
source_files += glob.glob("src/IO/*.cpp", recursive=False)
source_files += glob.glob("src/D3D11/*.cpp", recursive=False)
source_files += glob.glob("src/*.cpp", recursive=False)
source_files += glob.glob("src/Engine/*.cpp", recursive=False)
source_files += glob.glob("src/Input/*.cpp", recursive=False)

out = ''
# Changed files order to compile RHI files first
# TODO: Find a better way to do this
for filename in source_files[:-1:]:
    out += filename + ' '
out +=  source_files[-1]

print(out, end='')