#############################################################################
# Makefile for building: bin/textroom
# Generated by qmake (2.01a) (Qt 4.3.2) on: Sat Feb 23 19:28:19 2008
# Project:  textroom.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -unix -o Makefile textroom.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_SHARED -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -Isrc -Ibuild -Ibuild
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib -lQtGui -lQtCore -lpthread
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = build/

####### Files

SOURCES       = src/helpdialog.cpp \
		src/main.cpp \
		src/optionsdialog.cpp \
		src/textroom.cpp \
		src/searchdialog.cpp build/moc_helpdialog.cpp \
		build/moc_optionsdialog.cpp \
		build/moc_textroom.cpp \
		build/moc_searchdialog.cpp \
		build/qrc_textroom.cpp
OBJECTS       = build/helpdialog.o \
		build/main.o \
		build/optionsdialog.o \
		build/textroom.o \
		build/searchdialog.o \
		build/moc_helpdialog.o \
		build/moc_optionsdialog.o \
		build/moc_textroom.o \
		build/moc_searchdialog.o \
		build/qrc_textroom.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		textroom.pro
QMAKE_TARGET  = textroom
DESTDIR       = bin/
TARGET        = bin/textroom

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): build/ui_optionsdialog.h build/ui_textroom.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) bin/ || $(MKDIR) bin/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: textroom.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf
	$(QMAKE) -unix -o Makefile textroom.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile textroom.pro

dist: 
	@$(CHK_DIR_EXISTS) build/textroom1.0.0 || $(MKDIR) build/textroom1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/textroom1.0.0/ && $(COPY_FILE) --parents src/helpdialog.h src/optionsdialog.h src/textroom.h src/searchdialog.h build/textroom1.0.0/ && $(COPY_FILE) --parents resource/textroom.qrc build/textroom1.0.0/ && $(COPY_FILE) --parents src/helpdialog.cpp src/main.cpp src/optionsdialog.cpp src/textroom.cpp src/searchdialog.cpp build/textroom1.0.0/ && $(COPY_FILE) --parents ui/optionsdialog.ui ui/textroom.ui build/textroom1.0.0/ && (cd `dirname build/textroom1.0.0` && $(TAR) textroom1.0.0.tar textroom1.0.0 && $(COMPRESS) textroom1.0.0.tar) && $(MOVE) `dirname build/textroom1.0.0`/textroom1.0.0.tar.gz . && $(DEL_FILE) -r build/textroom1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: build/moc_helpdialog.cpp build/moc_optionsdialog.cpp build/moc_textroom.cpp build/moc_searchdialog.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) build/moc_helpdialog.cpp build/moc_optionsdialog.cpp build/moc_textroom.cpp build/moc_searchdialog.cpp
build/moc_helpdialog.cpp: src/helpdialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/helpdialog.h -o build/moc_helpdialog.cpp

build/moc_optionsdialog.cpp: build/ui_optionsdialog.h \
		src/optionsdialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/optionsdialog.h -o build/moc_optionsdialog.cpp

build/moc_textroom.cpp: build/ui_textroom.h \
		src/textroom.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/textroom.h -o build/moc_textroom.cpp

build/moc_searchdialog.cpp: src/searchdialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/searchdialog.h -o build/moc_searchdialog.cpp

compiler_rcc_make_all: build/qrc_textroom.cpp
compiler_rcc_clean:
	-$(DEL_FILE) build/qrc_textroom.cpp
build/qrc_textroom.cpp: resource/textroom.qrc \
		resource/images/splash.jpg
	/usr/bin/rcc -name textroom resource/textroom.qrc -o build/qrc_textroom.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: build/ui_optionsdialog.h build/ui_textroom.h
compiler_uic_clean:
	-$(DEL_FILE) build/ui_optionsdialog.h build/ui_textroom.h
build/ui_optionsdialog.h: ui/optionsdialog.ui
	/usr/bin/uic-qt4 ui/optionsdialog.ui -o build/ui_optionsdialog.h

build/ui_textroom.h: ui/textroom.ui
	/usr/bin/uic-qt4 ui/textroom.ui -o build/ui_textroom.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

build/helpdialog.o: src/helpdialog.cpp src/helpdialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/helpdialog.o src/helpdialog.cpp

build/main.o: src/main.cpp src/textroom.h \
		build/ui_textroom.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/main.o src/main.cpp

build/optionsdialog.o: src/optionsdialog.cpp src/optionsdialog.h \
		build/ui_optionsdialog.h \
		src/textroom.h \
		build/ui_textroom.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/optionsdialog.o src/optionsdialog.cpp

build/textroom.o: src/textroom.cpp src/textroom.h \
		build/ui_textroom.h \
		src/optionsdialog.h \
		build/ui_optionsdialog.h \
		src/helpdialog.h \
		src/searchdialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/textroom.o src/textroom.cpp

build/searchdialog.o: src/searchdialog.cpp src/searchdialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/searchdialog.o src/searchdialog.cpp

build/moc_helpdialog.o: build/moc_helpdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_helpdialog.o build/moc_helpdialog.cpp

build/moc_optionsdialog.o: build/moc_optionsdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_optionsdialog.o build/moc_optionsdialog.cpp

build/moc_textroom.o: build/moc_textroom.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_textroom.o build/moc_textroom.cpp

build/moc_searchdialog.o: build/moc_searchdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_searchdialog.o build/moc_searchdialog.cpp

build/qrc_textroom.o: build/qrc_textroom.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/qrc_textroom.o build/qrc_textroom.cpp

####### Install

install_target: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/bin/ 
	-$(INSTALL_PROGRAM) "bin/$(QMAKE_TARGET)" "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"
	-strip "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/bin/ 


install:  install_target  FORCE

uninstall: uninstall_target   FORCE

FORCE:

