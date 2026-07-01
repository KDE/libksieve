# SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

#[=======================================================================[.rst:
ECMInstalledLibraryCheck
------------------------

Generates a check to test the instanllation of a library for the self-containedness
of the CMake config files as well as the official public headers.

::

  ecm_add_installed_library_check(<library_target>
      [PACKAGE_NAME <package_name>}
      [PACKAGE_VERSION <package_version>]
      [PACKAGE_TARGET_NAMESPACE <package_target_namespace>]
      [NO_PACKAGE_TARGET_NAMESPACE])


``PACKAGE_NAME`` specifies the name of the CMake package to check for.
The default is "${PROJECT_NAME}".

``PACKAGE_VERSION`` specifies the version of the CMake package to check for.
The default is "${PROJECT_VERSION}" if set, otherwise none.

``PACKAGE_TARGET_NAMESPACE`` specifies what namespace the exported target
name of the library is placed in. The default is the value estimated for
the package name, unless ``NO_PACKAGE_TARGET_NAMESPACE`` is set.

``NO_PACKAGE_TARGET_NAMESPACE`` defines that the library target is exported
in the package without any namespace.


::

  ecm_installed_library_check_include_strings(<library_target>
      HEADERS <package_name>
      [PREFIX <prefix>])


``HEADERS`` specifies the header files whose base names will be available as
public include strings.

``PREFIX`` specifies a prefix, which consumers need to append next to the base names
of the headers passed to ``HEADERS``. The argument <prefix> is specified without a
trailing "/". Default is none.


Example usage:

.. code-block:: cmake

  # add a non-default target "MyLib_installed_library_check",
  # which will test for a CMake config file for "MyPackage",
  # at version "1.0",  with the imported library target
  # "MyPackage::MyLib" and whose include strings (headers)
  # can be used self-contained when linking the target
  ecm_add_installed_library_check(MyLib
      PACKAGE_NAME "MyPackage"
      PACKAGE_VERSION "1.0"
  )

  # for any <MLFoo> etc. includes
  ecm_installed_library_check_include_strings(MyLib
      HEADERS
          /absolute/path/MLFoo
          relative/path/MLBar
          # etc
  )

  # for any <ML/Foo> etc. includes
  ecm_installed_library_check_include_strings(MyLib
      HEADERS
          /absolute/path/Foo
          relative/path/Bar
          # etc
      PREFIX ML
  )

Since 6.28
#]=======================================================================]

cmake_policy(VERSION 3.29)

if(NOT TARGET all_installed_library_check)
    add_custom_target(all_installed_library_check)
endif()

# Purpose: testing basic self-containedness of public headers and cmake config files
# no testing of actual symbols or template methods
# TODO: support testing with other build metadata formats, like pkgconfig?
function(ecm_add_installed_library_check _target)
    set(options NO_PACKAGE_TARGET_NAMESPACE)
    set(oneValueArgs
        PACKAGE_NAME
        PACKAGE_VERSION
        PACKAGE_TARGET_NAMESPACE
    )
    set(multiValueArgs)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # argument checks
    if(ARG_UNPARSED_ARGUMENTS)
        message(
            FATAL_ERROR
            "Unknown keywords given to ecm_add_installed_library_check(): \"${ARG_UNPARSED_ARGUMENTS}\""
        )
    endif()
    if(ARG_NO_PACKAGE_TARGET_NAMESPACE AND ARG_PACKAGE_TARGET_NAMESPACE)
        message(
            FATAL_ERROR
            "ecm_add_installed_library_check cannot be called with both NO_PACKAGE_TARGET_NAMESPACE and PACKAGE_TARGET_NAMESPACE args."
        )
    endif()

    # setup data
    if(ARG_PACKAGE_NAME)
        set(_package_name "${ARG_PACKAGE_NAME}")
    else()
        set(_package_name "${PROJECT_NAME}")
    endif()
    if(ARG_PACKAGE_VERSION)
        set(_package_version ${ARG_PACKAGE_VERSION})
    else()
        if(PROJECT_VERSION)
            set(_package_version ${PROJECT_VERSION})
        else()
            set(_package_version)
        endif()
    endif()
    if(_package_version)
        set(_package_version_exact "EXACT")
    else()
        set(_package_version_exact)
    endif()
    if(ARG_PACKAGE_TARGET_NAMESPACE)
        set(_library_target_namespace ${ARG_PACKAGE_TARGET_NAMESPACE})
    else()
        if(ARG_NO_PACKAGE_TARGET_NAMESPACE)
            set(_library_target_namespace)
        else()
            set(_library_target_namespace ${_package_name})
        endif()
    endif()

    # prepare (generator) expressions
    set(_installed_library_check_dir "${CMAKE_CURRENT_BINARY_DIR}/${_target}_ECMInstalledLibraryCheck")
    set(_include_strings "$<TARGET_PROPERTY:${_target},ECM_INSTALLED_LIBRARY_INCLUDE_STRINGS>")
    set(_exported_target_name "$<TARGET_PROPERTY:${_target},EXPORT_NAME>")
    set(_library_target_name
        "${_library_target_namespace}$<IF:$<BOOL:${_exported_target_name}>,${_exported_target_name},${_target}>"
    )

    file(
        GENERATE OUTPUT "${_installed_library_check_dir}/CMakeLists.txt"
        CONTENT
            "cmake_minimum_required(VERSION 3.29)

project(InstalledLibraryCheck)

find_package(${_package_name} ${_package_version} ${_package_version_exact} CONFIG REQUIRED)

include(FeatureSummary)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

add_library(InstalledLibraryCheck MODULE)
target_link_libraries(InstalledLibraryCheck ${_library_target_name})

set(_include_strings
    ${_include_strings}
)

foreach(_include_sequence IN LISTS _include_strings)
    string(REPLACE \"/\" \"__\" _escaped_include_sequence \${_include_sequence})
    set(_source_file \"\${CMAKE_CURRENT_BINARY_DIR}/\${_escaped_include_sequence}.cpp\")
    file(GENERATE OUTPUT \${_source_file} CONTENT \"#include <\${_include_sequence}>\\n\")
    target_sources(InstalledLibraryCheck PRIVATE \${_source_file})
endforeach()
"
    )

    set(_check_target_name ${_target}_installed_library_check)
    add_custom_target(
        ${_check_target_name}
        COMMAND
            ${CMAKE_COMMAND} . --fresh
        COMMAND
            ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${_installed_library_check_dir}
    )

    add_dependencies(all_installed_library_check ${_check_target_name})
endfunction()

function(ecm_installed_library_check_include_strings _target)
    set(options)
    set(oneValueArgs PREFIX)
    set(multiValueArgs HEADERS)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    get_target_property(_names ${_target} ECM_INSTALLED_LIBRARY_INCLUDE_STRINGS)
    if(_names STREQUAL "_names-NOTFOUND")
        set(_names)
    endif()

    if(ARG_PREFIX)
        string(APPEND ARG_PREFIX "/")
    endif()
    foreach(_header IN LISTS ARG_HEADERS)
        cmake_path(GET _header FILENAME _name)
        list(APPEND _names "${ARG_PREFIX}${_name}")
    endforeach()

    list(APPEND _ecm_output_targets ${_out_targets})
    set_target_properties(
        ${_target}
        PROPERTIES
            ECM_INSTALLED_LIBRARY_INCLUDE_STRINGS
                "${_names}"
    )
endfunction()
