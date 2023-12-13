#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <vector>

using namespace std;

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

typedef struct {int x; int y;} Vector2;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
      interrupt_received = true;
}


class Snow {
private:
    Canvas *canvas;
    vector<Vector2> snowflakes;
    int counter;
    void render();
    void createSnowFlake();
    void updateSnowflakePos();
public:
    Snow(Canvas *canvas);
    void start();
};

