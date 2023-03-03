import glob

source_files = glob.glob("src/**/*.cpp", recursive=True)
out = ''
for filename in source_files:
    out += filename

print(out)