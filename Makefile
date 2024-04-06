CFLAGS = -std=c++17

CXXSOURCE = main.cpp \
	app/app_init.cpp \
	src/*.cpp \
	QRCode/src/qrcode.c \
	lodepng/lodepng.cpp

INC=-I -IQRCode/src -Ilodepng -Isrc -Iapp

TARGET = qrcode

COMPILER = g++

$(TARGET): $(CXXSOURCE)
	$(COMPILER) $(CFLAGS) $(INC) -o $(TARGET) $(CXXSOURCE) $(LDFLAGS)

.PHONY: test clean

test: $(TARGET)
	./$(TARGET) --t 2 --v 4 --str www.google.com --f test.png

clean:
	rm -f $(TARGET)