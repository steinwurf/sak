#! /usr/bin/env python
# encoding: utf-8

import os

# Necessary since we override different Contexts 
import waflib.extras.wurftools as wt

APPNAME = 'sak'
VERSION = '0.1'

wt.add_dependency('boost', 'git://github.com/steinwurf/external-waf-boost.git')
wt.add_dependency('gtest', 'git://github.com/steinwurf/external-waf-gtest.git', 'v1.6.0')

def load_helper(ctx, name):
    if ctx.is_system_dependency(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        ctx.load_dependency(name)

def options(opt):

    opt.load('wurftools')

    load_helper(opt, 'boost')
    load_helper(opt, 'gtest')

def configure(conf):

    conf.load('wurftools')

    load_helper(conf, 'boost')
    load_helper(conf, 'gtest')




def build(bld):

    bld.load('wurftools')

    load_helper(bld, 'boost')
    load_helper(bld, 'gtest')

    bld(includes = '.',
        export_includes = '.',
        name = 'sak_includes')

    # Build the tests
    bld.recurse('test')

    
    
