"""
So this is a test script designed to accompish two things:
	- run repsnapper with a large array of stl's and settings
	- test whether the gcode output matches what's expected (for the purpose of confirming refactoring changes made no functional change)
"""

import os

to_run = '..\\MSVC9\\Debug\\RepSnapper.exe'

def run_test(stl, config):
	(dir, stl_file) = os.path.split(stl)
	(stl_name, ext) = os.path.splitext(stl_file)
	(dir, config_file) = os.path.split(config)
	(config_name, ext) = os.path.splitext(config_file)
	out_name = stl_name + '.' + config_name + '.gcode'
	out = os.path.join('stls', 'output', out_name)

	command = ' '.join([to_run,'--no-gui','-i',stl,'-o',out,'-s',config])
	print command
	os.system(command)


stldir = 'stls'

def is_file(filename, subdir, exten):
	path = os.path.join(os.path.join(subdir,filename))
	(short,ext) = os.path.splitext(path)
	return os.path.isfile(path) and ext == exten

files = os.listdir(stldir)
stls = [filename for filename in files if is_file(filename,stldir,'.stl')]
stl_paths = [os.path.join(stldir, filename) for filename in stls]

print stl_paths

configsdir = os.path.join('stls','configs')
files = os.listdir(configsdir)
configs = [filename for filename in files if is_file(filename,configsdir,'.xml')]
config_paths = [os.path.join(configsdir, filename) for filename in configs]

print config_paths

for config in config_paths:
	for stl in stl_paths:
		run_test(stl, config)
