"""
Quick n dirty sdl detection
"""

import os, glob, types
from waflib.Configure import conf


def options(opt):
	opt.add_option('--sdl', type='string', help='path to sdl', dest='sdl')


@conf
def check_sdl(conf):
	if conf.options.sdl:
		includes_check = [conf.options.sdl + '/include']
		libs_check = [conf.options.sdl + '/lib']
	else:
		includes_check = ['/usr/local/include', '/usr/include']
		libs_check = ['/usr/local/lib', '/usr/lib', '/usr/lib/x86_64-linux-gnu/']

	try:
		conf.start_msg('Checking for SDL C++ includes')
		res = conf.find_file('SDL/SDL.h', includes_check)
		conf.end_msg('ok')
	except:
		conf.end_msg('Not found', 'RED')
		return 1
	conf.start_msg('Checking for SDL C++ libs')
	found = False
	for lib in ['libSDL.so']: #, 'libSDL_image.so']:
		try:
			found = found or conf.find_file(lib, libs_check)
		except:
			continue
	if not found:
		conf.end_msg('Not found', 'RED')
		return 1
	else:
		conf.end_msg('ok')
		conf.env.INCLUDES_SDL = includes_check
		conf.env.LIBPATH_SDL = libs_check
		conf.env.DEFINES_SDL = ['USE_SDL']
		conf.env.LIB_SDL = ['SDL']#, 'SDL_image']
	return 1
