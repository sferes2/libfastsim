"""
Quick n dirty sdl detection
"""

import os, glob, types
from waflib.Configure import conf


def options(opt):
	opt.add_option('--sdl', type='string', help='path to sdl', dest='sdl')


@conf
def check_sdl(conf):
	conf.start_msg('Checking for SDL (1.2 - sdl-config)')

	try:
		conf.check_cfg(path='sdl-config', args='--cflags --libs', package='', uselib_store='SDL')
	except:
		conf.end_msg('sdl-config not found', 'RED')
		return 1
	conf.end_msg('ok')
	conf.env.DEFINES_SDL += ['USE_SDL']
	return 1
