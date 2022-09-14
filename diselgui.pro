QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
CONFIG += no_testcase_installs

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    attrlineitem.cpp \
    baseblitem.cpp \
    basegraphicsitem.cpp \
    basergitem.cpp \
    booleanlatticemodel.cpp \
    main.cpp \
    mainwindow.cpp \
    newatomdialog.cpp \
    newconceptdialog.cpp \
    newedgedialog.cpp \
    newrelationdialog.cpp \
    rootedgraphmodel.cpp \
    utils.cpp

HEADERS += \
    attrlineitem.h \
    baseblitem.h \
    basegraphicsitem.h \
    basergitem.h \
    booleanlatticemodel.h \
    mainwindow.h \
    newatomdialog.h \
    newconceptdialog.h \
    newedgedialog.h \
    newrelationdialog.h \
    rootedgraphmodel.h \
    utils.h

FORMS += \
    mainwindow.ui \
    newatomdialog.ui \
    newconceptdialog.ui \
    newedgedialog.ui \
    newrelationdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../cpp_language/DISELAPI/DISELAPI/x64/release/ -lDISELAPILIB
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../cpp_language/DISELAPI/DISELAPI/x64/debug/ -lDISELAPILIB
else:unix: LIBS += -L$$PWD/../../cpp_language/DISELAPI/DISELAPI/x64/ -lDISELAPILIB

INCLUDEPATH += $$PWD/../../cpp_language/DISELAPI/DISELAPI/DISELAPILIB
DEPENDPATH += $$PWD/../../cpp_language/DISELAPI/DISELAPI/DISELAPILIB

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../cpp_language/DISELAPI/DISELAPI/x64/release/libDISELAPILIB.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../cpp_language/DISELAPI/DISELAPI/x64/debug/libDISELAPILIB.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../cpp_language/DISELAPI/DISELAPI/x64/release/DISELAPILIB.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../cpp_language/DISELAPI/DISELAPI/x64/debug/DISELAPILIB.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../../cpp_language/DISELAPI/DISELAPI/x64/libDISELAPILIB.a

win32:CONFIG(release, debug|release): LIBS += -LC:/src/vcpkg/packages/tinyxml2_x64-windows/lib/ -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/src/vcpkg/packages/tinyxml2_x64-windows/debug/lib/ -ltinyxml2d
else:unix: LIBS += -LC:/src/vcpkg/packages/tinyxml2_x64-windows/lib/ -ltinyxml2

INCLUDEPATH += C:/src/vcpkg/packages/tinyxml2_x64-windows/include
DEPENDPATH += C:/src/vcpkg/packages/tinyxml2_x64-windows/include

#unix|win32: LIBS += -L'C:/Program Files/Graphviz/lib/' -lgvc -lcdt -lcgraph

#INCLUDEPATH += 'C:/Program Files/Graphviz/include/graphviz'
#DEPENDPATH += 'C:/Program Files/Graphviz/include/graphviz'
