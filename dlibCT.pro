QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = dlibCT
TEMPLATE = app
CONFIG += c++11
DEFINES += "USE_AVX_INSTRUCTIONS=1" NDEBUG DLIB_PNG_SUPPORT DLIB_JPEG_SUPPORT DLIB_USE_BLAS DLIB_USE_LAPACK

SOURCES += src/main.cpp\
        src/controller.cpp\
        src/counter.cpp\
        src/gui.cpp\
        src/id_generator.cpp\
        src/line.cpp\
        src/metadata_editor.cpp\
        src/root.cpp\
        src/tracker.cpp

HEADERS  += src/controller.h\
                src/counter.h\
                src/gui.h\
                src/identifiert.h\
                src/id_generator.h\
                src/line.h\
                src/metadat_editor.h\
                src/root.h\
                src/tracker.h

FORMS    += 

LIBS += -L/usr/local/lib -lopencv_videoio -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_objdetect -lopencv_imgproc -lopencv_core -lopencv_features2d -l opencv_flann -lopencv_calib3d -ldlib\
	-L/usr/lib -ljpeg -lX11 -lblas -llapack

