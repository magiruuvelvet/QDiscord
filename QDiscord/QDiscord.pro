#-------------------------------------------------
#
# Project created by QtCreator 2016-05-01T17:55:58
#
#-------------------------------------------------

TEMPLATE = subdirs

SUBDIRS += src
CONFIG(debug, debug|release) {
    SUBDIRS += tests
    tests.depends = src
}

OTHER_FILES += Doxyfile

include(doc/doc.pri)
