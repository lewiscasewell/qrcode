CFLAGS = -std=c++17

# Assuming QRCode/src/qrcode.c should be compiled as C++, given your setup. If not, adjustments are needed.
CXXSOURCE = main.cpp \
	app/app_init.cpp \
	src/*.cpp \
	QRCode/src/qrcode.c \
	lodepng/lodepng.cpp

# Corrected include directories, removed the stray -I
INC = -IQRCode/src -Ilodepng -Isrc -Iapp

TARGET = qrcode_app

COMPILER = g++

$(TARGET): $(CXXSOURCE)
	$(COMPILER) $(CFLAGS) $(INC) -o $(TARGET) $(CXXSOURCE) $(LDFLAGS)

.PHONY: test clean

test: $(TARGET)
	./$(TARGET) --t 2 --v 4 --str www.lewiscasewell.com --f lewiscasewell.png

clean:
	rm -f $(TARGET)