# use this to remove libs/ artifacts fetched by CMake.
import shutil
import os

for it in [ f.path for f in os.scandir('./libs/') if f.is_dir()]:
  if it == './libs/glad':
    continue
  shutil.rmtree(it)
  os.makedirs(it)
  with open(f'{it}/.gitkeep', 'w') as file_stream:
    pass
