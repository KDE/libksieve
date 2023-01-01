# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
macro(add_sieveutil_test _source _additionalsource)
  set( _test ${_source} ${_additionalsource})
  get_filename_component( _name ${_source} NAME_WE )
  add_executable( ${_name} ${_test} ${_name}.h)
  add_test(NAME ${_name} COMMAND ${_name})
  ecm_mark_as_test(sievescriptdebugger-${_name})
  target_link_libraries( ${_name}  KF5::KSieveUi Qt::Test KF5::MailTransport KF5::I18n)
endmacro()

add_sieveutil_test(sieveaccounttest.cpp "")
add_sieveutil_test(findaccountinfojobtest.cpp "akonadiimapsettinginterfacetest.cpp")
add_sieveutil_test(regexpconvertertest.cpp "")
