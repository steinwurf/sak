News for sak
============

This file lists the major changes between versions. For a more detailed list
of every change, see the Git log.

10.2.1
------
* Bug: Fixed bug in the conversion functions for big endian platforms.
  Reported by Martin Hundebøll.

10.2.0
------
* Minor: Added random_buffer class that can be filled and verified using an embedded random seed
* Minor: Updated to waf-tools 2
* Minor: Update to waf 1.7.10
* Minor: Remove unused boost::signals
* Minor: Conforming to new coding style

10.1.0
------
* Added the ceil_division.hpp for ceiled integer division.

10.0.0
------
* The endian_buffer class has been replaced with an endian_stream that
  provides a stream-like interface over a fixed-size buffer.
* sak::storage() can be created directly from an std::string

9.1.0
-----
* Added sak::equal() for comparing sak::const_storage objects.

9.0.0
------
* Updated to waf 1.7.9 and started using the wurf_install_path tool.
* Removing sak::object and sak::object_id and using typeid for type
  identification in the object registry.

8.1.0
-----
* Updated to use the new external-waf-tools repository.
* Added offset function to storage classes & fixed bug in the split_storage
  function.

8.0.0
-----
* Allow setting a factory function on the object registry
* Object registry is no longer a singleton, changed factory build methods to
  take accept object registry reference.
* Added random input stream.
* Use new mkspec system to building

7.0.0
-----
* Changed object model to no longer use category

6.1.0
-----
* Adding streams, e.g. buffer_input_stream, file_input_stream.

6.0.0
-----
* Bump major version due to an backwards incompatible change in the
  storage.hpp code. Where we have removed the storage_list() functions.

5.0.0
-----
* Bump major version due to an backwards incompatible change in the
  object_registry code.

4.4.1
-----
* Fix broken backwards compatibility introduced without bumping the
  major version number.

4.4.0
-----
* Added buffer object for simple buffer management.

4.3.0
-----
* Added the object registry, which allows management and access to
  object factories.

4.2.2
-----
* Fixed custom allocator support for C++11. For win32 a preprocessor define is
  used until variadic template is supported in Visual Studio.

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

