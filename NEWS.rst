News for sak
============

This file lists the major changes between versions. For a more detailed list
of every change, see the Git log.

tbd (master)
------------
* tbd

4.2.1
-----
* Fixed missing include in object.h

4.2.0
-----
* Added object agregation functionality

4.1.0
------------
* Adding aligned allocator + several utility functions for dealing
  with aligned memory.

4.0.0
-----
* Changing header extension to .hpp from .h which makes it easier to
  differentiate between C and C++ code.
* Added ``endian_buffer`` class for easy serializing and deserializing of
  multibyte values
* Added the ``duplex_buffer`` class as a container for data which supports
  efficient prepending and appending of data.

3.0.0
-----
* To avoid having include path point to the root of the project we have
  moved the source files to src/sak instead of just sak/.
* Removed obsolete code benchmarking functionality, now replaced by the
  cxx-gauge project.
* Added storage implementation.

2.0.0
-----
* Updated Waf build tool to handle dependencies better. The new tools
  used are not compatible with the old versions so we have to bump the
  major version.

1.1.1
-----
* Updated Waf build tool
* Bumped dependency version for gtest to 1.0.3-gtest_1_6_0
* Bumped dependency version for boost to 1.1.2-boost_1_48_0

1.1.0
-----
* Added helper functions for profiling code
* Bumped dependency version for gtest to 1.0.2-gtest_1_6_0
* Bumped dependency version for boost to 1.1.1-boost_1_48_0
* Fixed win32 link and cxx flags

