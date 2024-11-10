# use this to remove libs/ artifacts fetched by CMake.
import shutil
import os
import stat

def remove_readonly(func, path, exc_info):
  os.chmod(path, stat.S_IWRITE)
  func(path)

for it in [ f.path for f in os.scandir('./libs/') if f.is_dir()]:
  if it == './libs/glad':
    continue
  shutil.rmtree(it, onerror=remove_readonly)
  os.makedirs(it)
  with open(f'{it}/.gitkeep', 'w') as file_stream:
    pass
