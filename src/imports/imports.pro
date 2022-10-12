TEMPLATE = subdirs

#SUBDIRS += folderlistmodel particles gestures
SUBDIRS += folderlistmodel particles
contains(QT_CONFIG, opengl): SUBDIRS += shaders

