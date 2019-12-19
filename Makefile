CC		= g++
CFLAGS	= -std=gnu++17
TARGET	= FaceDetector
SRC		= FaceDetectorMain.cpp FaceDetector.cpp jsoncpp.cpp
LDLIBS	= -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_objdetect

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(TARGET)