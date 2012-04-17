#! /usr/bin/env python
# encoding: utf-8

import os

APPNAME = 'sak'
VERSION = '1.1.1'

def recurse_helper(ctx, name):
    if not ctx.has_dependency_path(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        p = ctx.dependency_path(name)
        ctx.recurse(p)

def options(opt):

    opt.load('toolchain_cxx')
    opt.load('bundle_dependency')

    import waflib.extras.dependency_bundle as bundle
    import waflib.extras.dependency_resolve as resolve

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'gtest',
            git_repository = 'git://github.com/steinwurf/external-gtest.git',
            major_version = 1))

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'boost',
            git_repository = 'git://github.com/steinwurf/external-boost.git',
            major_version = 1))

def configure(conf):

    if conf.is_toplevel():

        conf.load('toolchain_cxx')
        conf.load('dependency_bundle')

        recurse_helper(conf, 'boost')
        recurse_helper(conf, 'gtest')

def build(bld):

    bld(includes = '.',
        export_includes = '.',
        name = 'sak_includes')

    if bld.is_toplevel():

        bld.load('dependency_bundle')

        recurse_helper(bld, 'boost')
        recurse_helper(bld, 'gtest')

        # Only build test are executed from the
        # top-level wscript i.e. not when included as a dependency
        # in a recurse call

        bld.recurse('test')



