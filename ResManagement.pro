QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/handler/ResManagementPanelDirTreeHandler.h \
    src/handler/ResManagementPanelFileListHandler.h \
    src/handler/ResManagementPanelFilePathNaviHandler.h \
    src/model/FileItemFactory.h \
    src/model/FileTreeModel.h \
    src/model/FileItem.h \
    src/model/FileItem_Fwd.h \
    src/model/FileTreeModel.h \
    src/model/FileType.h \
    src/model/refined_fileitems/FileItemOnlyWithName.h \
    src/model/refined_fileitems/MtgFileItem.h \
    src/model/refined_fileitems/NfxFileItem.h \
    src/panel/ResManagementPanel.h \
    src/handler/ResManagementPanelHandler.h \
    src/handler/ResManagementPanelHandlerTypes.h \
    src/panel/SearchSettingForm.h \
    src/utils/FunctionPerformer.h

SOURCES += \
    src/handler/ResManagementPanelDirTreeHandler.cpp \
    src/handler/ResManagementPanelFileListHandler.cpp \
    src/handler/ResManagementPanelFilePathNaviHandler.cpp \
    src/main.cpp \
    src/model/FileItem.cpp \
    src/model/FileItemFactory.cpp \
    src/model/FileTreeModel.cpp \
    src/model/FileType.cpp \
    src/model/refined_fileitems/FileItemOnlyWithName.cpp \
    src/model/refined_fileitems/MtgFileItem.cpp \
    src/model/refined_fileitems/NfxFileItem.cpp \
    src/panel/ResManagementPanel.cpp \
    src/model/FileTreeModel.cpp \
    src/panel/SearchSettingForm.cpp \
    src/utils/FunctionPerformer.cpp

RESOURCES += \
    res/ResManagement.qrc

FORMS += \
    res/ResManagementPanel.ui \
    res/SearchSettingForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
