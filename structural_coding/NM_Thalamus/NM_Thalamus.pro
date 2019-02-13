TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = release_binary

SOURCES +=  Thalamic_Column.cpp \
			Thalamus.cpp	\
			Thalamus_mex.cpp

HEADERS +=  Data_Storage.h	\
			ODE.h		\
			Random_Stream.h	\
			Thalamic_Column.h

SOURCES -= Thalamus_mex.cpp

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3
