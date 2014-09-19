#!/usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='libfastsim'
srcdir = '.'
blddir = 'build'

import copy
import os, sys, commands


def detect_sdl(conf):
    conf.start_msg('Checking for SDL')
    ret = conf.find_program('sdl-config')
    if not ret:
        conf.end_msg('ERROR')
        return 0
    res = commands.getoutput('sdl-config --cflags --libs')
    conf.parse_flags(res, uselib_store='SDL')
    conf.end_msg('ok')
    return 1


def options(opt):
        opt.load('compiler_cxx boost waf_unit_test')
        opt.load('compiler_c')
	
def configure(conf):
    	print("configuring b-optimize")
    	conf.load('compiler_cxx boost waf_unit_test')
        conf.load('compiler_c')
	detect_sdl(conf)
	common_flags = "-Wall -DUSE_SDL "

	cxxflags = conf.env['CXXFLAGS']
#	conf.check_boost(lib='',
#            min_version='1.35')
 
	# release
        opt_flags = common_flags + ' -O3 -msse2 -ggdb3 -g'
        conf.env['CXXFLAGS'] = cxxflags + opt_flags.split(' ')
        print conf.env['CXXFLAGS']

def build(bld):
	bld.recurse('src/')
	
