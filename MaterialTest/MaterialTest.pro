TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/GLFW

INCLUDEPATH += $$PWD/emscripten

LIBS += -L$$PWD/GLFW -lglfw3 -lopengl32 -lgdi32

SOURCES += main.cpp \
            glad.c \
            object.cpp \
            shader.cpp \
            skeletalanimation.cpp

HEADERS += \
    object.h \
    shader.h \
    skeletalanimation.h
