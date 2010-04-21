TEMPLATE = app
TARGET = textroom
DEPENDPATH += . resource src ui
INCLUDEPATH += . src
DESTDIR += bin
OBJECTS_DIR += build
MOC_DIR += build
UI_DIR += build
RCC_DIR += build
HEADERS += src/helpdialog.h \
 src/optionsdialog.h \
 src/textroom.h \
 src/searchdialog.h \
 src/font.h
FORMS += ui/optionsdialog.ui ui/textroom.ui ui/helpdialog.ui ui/font.ui
SOURCES += src/helpdialog.cpp \
 src/main.cpp \
 src/optionsdialog.cpp \
 src/textroom.cpp \
 src/searchdialog.cpp \
 src/font.cpp
RESOURCES += resource/textroom.qrc
CONFIG += release

INSTALLS += data \
 target \
 documentation \
 icon \
 icon-doc \
 desktop \
 kdedesktop \
 kdemime \
 gnomemime \
 gnomekeys
data.path = /usr/local/share/textroom
data.files = resource/sounds/*
target.path = /usr/local/bin
documentation.path = /usr/local/textroom/doc
documentation.files = docs/*
icon.path = /usr/share/pixmaps
icon.files = resource/images/textroom.png
icon-doc.path = /usr/share/pixmaps
icon-doc.files = resource/images/textroom-doc.png
desktop.path = /usr/share/applications
desktop.files = resource/desktop/TextRoom.desktop
kdedesktop.path = ~/.local/share/applications
kdedesktop.files = resource/desktop/TextRoom.desktop
kdemime.path = ~/.kde/share/mimelnk/text
kdemime.files = resource/desktop/textroom.desktop
gnomemime.path = ~/.gnome/mime-info
gnomemime.files = resource/desktop/textroom.mime
gnomekeys.path = ~/.gnome/mime-info
gnomekeys.files = resource/desktop/textroom.keys
LIBS += -lSDL_mixer -lSDL
