CONFIG += staticlib

QT += opengl

SOURCES += \
    tools/tpan.cpp \
    tools/tplane.cpp \
    tools/tbox.cpp \
    tools/tcylinder.cpp \
    tools/tmove.cpp \
    tools/torbit.cpp \
    tools/trotate.cpp \
    tools/trotatecamera.cpp \
    tools/tscale.cpp \
    tools/tselect.cpp \
    tools/ttriangle.cpp \
    tools/tvertex.cpp \
    tools/tzoom.cpp \
    tools/tellipse.cpp \
    tools/tsphere.cpp \
                        \
    gui/mycheckbox.cpp \
    gui/mycombobox.cpp \
    gui/mypushbutton.cpp \
    gui/myspinbox.cpp \
    gui/mycheckboxmw.cpp \
    gui/mylabel.cpp \
    gui/mypushbuttonmw.cpp \
    gui/myframe.cpp \
                        \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    tool.cpp \
    vertex.cpp \
    triangle.cpp \
    glwidget.cpp \
    camera.cpp \
    toolwithwidget.cpp \
    transformingtool.cpp \
    toolwithpivot.cpp \
    mathfunctions.cpp \
    creatingtool.cpp \
    record.cpp \
    journal.cpp \
    elementcontainer.cpp \
    element.cpp


HEADERS += \
    tools/tpan.h \
    tools/tbox.h \
    tools/tcylinder.h \
    tools/tmove.h \
    tools/torbit.h \
    tools/tplane.h \
    tools/trotate.h \
    tools/trotatecamera.h \
    tools/tscale.h \
    tools/tselect.h \
    tools/ttriangle.h \
    tools/tvertex.h \
    tools/tzoom.h \
    tools/tellipse.h \
    tools/tsphere.h \
                        \
    gui/mycheckbox.h \
    gui/mycombobox.h \
    gui/mypushbutton.h \
    gui/myspinbox.h \
    gui/mycheckboxmw.h \
    gui/mylabel.h \
    gui/mypushbuttonmw.h \
    gui/myframe.h \
                    \
    mainwindow.h \
    model.h \
    tool.h \
    vertex.h \
    triangle.h \
    glwidget.h \
    camera.h \
    toolwithwidget.h \
    transformingtool.h \
    toolwithpivot.h \
    mathfunctions.h \
    creatingtool.h \
    record.h \
    journal.h \
    elementcontainer.h \
    element.h


RESOURCES += \
    shaders.qrc


# TODO:
#
# options (colors)
# about
# panels can detatch
# all tools should be in File, Edit...
# model info
# open popular formats
# textures
# light
# selecting faces in textured

# basic tools: extrude, bend, cut ...

# in GLWidget::draw vertices should not repeat! - fix after "textures"
# select -> DRAW -> triangles - add bool checked[] not to repeat vertices (or better solution), find similar mistakes; - fix after "triangles selected by clicking or drawing rectangle inside them"
# check STAGE2 in TBox - should not work but works

# textures don't work
# tCylinder->widget() is not hidden as it is the biggest widget
