#! /usr/bin/env python
# encoding: utf-8

import os

# Necessary since we override different Contexts 
import waflib.extras.wurftools

APPNAME = 'sak'
VERSION = '0.1'

def options(opt):
    opt.load('wurftools')

    opt.add_dependency('boost', 'git://github.com/steinwurf/external-waf-boost.git')
    opt.add_dependency('gtest', 'git://github.com/steinwurf/external-waf-gtest.git', 'v1.6.0')

    if not opt.bundle_dependency('gtest'):
        opt.add_option('--gtest-path')
    else:
        opt.recurse_dependency('gtest')

    if not opt.bundle_dependency('boost'):
        opt.add_option('--boost-path')
    else:
        opt.recurse_dependency('boost')


def configure(conf):

    conf.load('wurftools')

    if not conf.bundle_dependency('gtest'):
        if not conf.options.gtest_path:
            conf.fatal('Either bundle gtest of specify its path')
        else:
            conf.recurse(conf.options.gtest_path)
            conf.env.GTEST_PATH = conf.options.gtest_path
    else:
        conf.recurse_dependency('gtest')

    if not conf.bundle_dependency('boost'):
        if not conf.options.boost_path:
            conf.fatal('Either bundle boost of specify its path')
        else:
            conf.recurse(conf.options.boost_path)
            conf.env.BOOST_PATH = conf.options.boost_path
    else:
        conf.recurse_dependency('boost')



def build(bld):

    bld.load('wurftools')

    if not bld.bundle_dependency('gtest'):
        if not bld.env.GTEST_PATH: bld.fatal('Either bundle gtest of specify its path during "./waf configure"')
        else: bld.recurse(bld.env.GTEST_PATH)
    else:
        bld.recurse_dependency('gtest')

    if not bld.bundle_dependency('boost'):
        if not bld.env.BOOST_PATH: bld.fatal('Either bundle boost of specify its path during "./waf configure"')
        else: bld.recurse(bld.env.BOOST_PATH)

    else:
        bld.recurse_dependency('boost')

    bld(includes = '.',
        export_includes = '.',
        name = 'sak_includes')

    # Build the tests
    bld.recurse('test')

    
    
