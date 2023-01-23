# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

include_directories(${libksieve_SOURCE_DIR}
        ${libksieve_SOURCE_DIR}/src/ksieveui/scriptsparsing
        )


set(editorsieve_gui_SRCS editorsieve_gui.cpp ../../tests/capability.cpp editorsieve_gui.h ../../tests/capability.h)
add_executable(editorsieve_gui ${editorsieve_gui_SRCS}) 
target_link_libraries(editorsieve_gui
  KF5::KSieveUi
  KF5::PimCommon
  KF${KF_MAJOR_VERSION}::I18n
  Qt::PrintSupport
  KF${KF_MAJOR_VERSION}::SyntaxHighlighting
)

