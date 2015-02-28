SOURCES = \
main.cpp \
RenderTexture.cpp \
GLWindow.cpp \
Stroke.cpp \
GLPainterlyObj.cpp \
GLPainterlyObjCg.cpp \
GLPainterlyObjFile.cpp \
GLPainterlyObjFilters.cpp \
GLPainterlyObjInit.cpp \
GLPainterlyObjOptions.cpp \
GLPainterlyObjPaint.cpp  \
GLPainterlyObjStyles.cpp \
GLPainterlyObjUtility.cpp

HEADERS = GLWindow.h GLPainterlyObj.h Color.h Point.h RenderTexture.h Stroke.h
CONFIG += qt warn_on release opengl thread
DEFINES += QT_CLEAN_NAMESPACE
#INCLUDEPATH += .;../glew/include
INCLUDEPATH += .;/home/max/stc/max/glew/include
#unix:LIBS += -lCgGL -lCg -lXmu -lXi -lpthread -lGL -lGLU `Magick++-config --ldflags --libs` -lglut -lGLEW -L../glew/lib
unix:LIBS += -lCgGL -lCg -lXmu -lXi -lpthread -lGL -lGLU -lglut -lGLEW -L/home/max/stc/max/glew/lib
win32:LIBS +=
TARGET = main
