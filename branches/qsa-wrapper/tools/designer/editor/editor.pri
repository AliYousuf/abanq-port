HEADERS += \
	$$EDITOR_BRANCH/editor.h  \
	$$EDITOR_BRANCH/parenmatcher.h  \
	$$EDITOR_BRANCH/completion.h \
	$$EDITOR_BRANCH/viewmanager.h \
	$$EDITOR_BRANCH/markerwidget.h\
	$$EDITOR_BRANCH/conf.h \
	$$EDITOR_BRANCH/browser.h \
	$$EDITOR_BRANCH/arghintwidget.h \
	$$EDITOR_BRANCH/cindent.h

SOURCES += \
	$$EDITOR_BRANCH/editor.cpp \
	$$EDITOR_BRANCH/parenmatcher.cpp  \
	$$EDITOR_BRANCH/completion.cpp \
	$$EDITOR_BRANCH/viewmanager.cpp \
	$$EDITOR_BRANCH/markerwidget.cpp \
	$$EDITOR_BRANCH/conf.cpp \
	$$EDITOR_BRANCH/browser.cpp \
	$$EDITOR_BRANCH/arghintwidget.cpp \
	$$EDITOR_BRANCH/cindent.cpp \
	$$EDITOR_BRANCH/yyindent.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3 += \
	$$EDITOR_BRANCH/preferences.ui

INCLUDEPATH	+= $$QT_SOURCE_TREE/tools/designer/interfaces
#The following line was inserted by qt3to4
QT += qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3

