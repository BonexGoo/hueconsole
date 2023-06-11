TARGET = hueconsole
TEMPLATE = app

QT_ENABLE_GRAPHICS = ok #ok #no
QT_ADD_PLUGINS_FIREBASE = no #ok #no
QT_PACKAGE_RPATH = no #ok #no

!include(../../Boss2D/project/boss2d.pri) {
    error("Couldn't find the boss2d.pri file...")
}

wasm{
    QT -= bluetooth
    QMAKE_LFLAGS += -s TOTAL_MEMORY=26214400
}

INCLUDEPATH += ../source-gen
HEADERS += ../source-gen/boss_config.h
HEADERS += ../source-gen/boss_assets.hpp
HEADERS += ../source-gen/boss_buildtag.hpp

INCLUDEPATH += ../source
SOURCES += ../source/huecoding.cpp
HEADERS += ../source/huecoding.h
SOURCES += ../source/hueconsole.cpp
HEADERS += ../source/hueconsole.hpp
SOURCES += ../source/main.cpp
SOURCES += ../source/resource.cpp
HEADERS += ../source/resource.hpp
SOURCES += ../source/base64/base64.cpp
HEADERS += ../source/base64/base64.h

INCLUDEPATH += ../huecoding
SOURCES += ../huecoding/example.cpp
SOURCES += ../huecoding/hayoon.cpp
SOURCES += ../huecoding/kangsa.cpp
SOURCES += ../huecoding/koeunbin.cpp
SOURCES += ../huecoding/leehyunjoo.cpp
SOURCES += ../huecoding/leeseungah.cpp

ASSETS_IMAGE.files += ../assets/image
ASSETS_IMAGE.path = /assets

win32{
    RC_ICONS += ../common/windows/main.ico
}

macx{
    QMAKE_BUNDLE_DATA += ASSETS_IMAGE
    QMAKE_INFO_PLIST = $$PWD/../common/macx/Info.plist
    QMAKE_ASSET_CATALOGS += $$PWD/../common/macx/Assets.xcassets
    QMAKE_ASSET_CATALOGS_APP_ICON = AppIcon
}

ios{
    QMAKE_BUNDLE_DATA += ASSETS_IMAGE
    QMAKE_INFO_PLIST = $$PWD/../common/ios/Info.plist
    QMAKE_ASSET_CATALOGS += $$PWD/../common/ios/Assets.xcassets
    QMAKE_ASSET_CATALOGS_APP_ICON = AppIcon
}

android{
    INSTALLS += ASSETS_IMAGE
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../common/android
}
