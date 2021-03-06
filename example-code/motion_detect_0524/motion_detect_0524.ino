//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

// include the relevant libraries
#include <FS.h>
#include <SPIFFS.h>
#include <eloquentarduino.h>
#include <eloquentarduino/io/serial_print.h>
#include <eloquentarduino/vision/camera/ESP32Camera.h>
#include <eloquentarduino/vision/io/decoders/Red565RandomAccessDecoder.h>
#include <eloquentarduino/vision/processing/MotionDetector.h>
#include <eloquentarduino/vision/processing/downscaling/Downscaler.h>
#include <eloquentarduino/vision/processing/downscaling/center.h>
#include <eloquentarduino/vision/io/writers/JpegWriter.h>

// define the source format and the downscaled dimensions
#define FRAME_SIZE FRAMESIZE_QVGA
#define PIXFORMAT PIXFORMAT_RGB565
#define W 320
#define H 240
#define w 32
#define h 24
// tweak these values as per your need
#define DIFF_THRESHOLD 15
#define MOTION_THRESHOLD 0.08
#define STOP_PUB_TRIGGER 240 // 120(s)

// delete the second definition if you want to turn on code benchmarking
#define timeit(label, code) { uint32_t start = millis(); code; uint32_t duration = millis() - start; eloquent::io::print_all("It took ", duration, " millis for ", label); }
#define timeit(label, code) code;

#define SIGNAL_PIN 14
int timeCount = 0;
int state = 1;

using namespace Eloquent::Vision;

camera_fb_t *frame;
Camera::ESP32Camera camera(PIXFORMAT);
IO::Decoders::Red565RandomAccessDecoder decoder;
Processing::Downscaling::Center<W / w, H / h> strategy;
Processing::Downscaling::Downscaler<W, H, w, h> downscaler(&decoder, &strategy);
Processing::MotionDetector<w, h> motion;
IO::Writers::JpegWriter<W, H> jpegWriter;

int stop_count = 0;

void setup() {
    Serial.begin(115200);
    pinMode(SIGNAL_PIN, OUTPUT);
    delay(1000);
    Serial.println("Begin");

    camera.begin(FRAME_SIZE);
    // set how much a pixel value should differ to be considered as a change
    motion.setDiffThreshold(DIFF_THRESHOLD);
    // set how many pixels (in percent) should change to be considered as motion
    motion.setMotionThreshold(MOTION_THRESHOLD);
    // prevent consecutive triggers
    motion.setDebounceFrames(5);
}

void loop() {
    capture();
    eloquent::io::print_all(motion.changes(), " pixels changed");
    
    if (motion.triggered()) {
        Serial.println("Motion detected");
        timeCount = 80;
    } else {
        timeCount--;
    }
    if (timeCount == -10){timeCount = 0;}
    Serial.println(timeCount);
    if (timeCount<1){digitalWrite(SIGNAL_PIN,HIGH);}
    else {digitalWrite(SIGNAL_PIN,LOW);}
}

void capture() {
    uint8_t downscaled[w * h];

    // capture image
    timeit("capture frame", frame = camera.capture());

    // scale image from size H * W to size h * w
    timeit("downscale", downscaler.downscale(frame->buf, downscaled));

    // detect motion on the downscaled image
    timeit("motion detection", motion.detect(downscaled));
}
