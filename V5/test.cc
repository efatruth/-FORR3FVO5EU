#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct {int x; int y;;} Vector2;

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
	  interrupt_received = true;
}

int Distance(Vector2 from, Vector2 to)
{
	return (abs(from.x - to.x) + abs(from.y - to.y)) / 2;
}

int Distance(Vector2 *from, Vector2 to)
{
	return (abs(from->x - to.x) + abs(from->y - to.y)) / 2;
}

int Distance(int from, int to)
{
	return abs(from-to);
}

void changeInt(int *from, int *to)
{
	if (to > from) from++;
	else if (to < from) from--;
}

static void setup()
{

}

void moveTo(Vector2 *pos, Vector2 dest, Canvas *canvas)
{
	int counter = 0;
	while (Distance(pos, dest) > 5)
	{
		if (Distance(pos->x, dest.x) > 1) pos->x++;
		if (Distance(pos->y, dest.y) > 1) pos->y++;
		canvas->Clear();
		for (int x = 0; x < 4; x++)
		{
			canvas->SetPixel(x + pos->x, pos->y,
				255, 0, 0);
			canvas->SetPixel(x + pos->x - 1, pos->y,
				255, 0, 0);
		}
		if (counter == 1500)
		{
			break;
		}
		counter++;
		usleep(10000);
	}
}

static void draw(Canvas *canvas)
{
	/*canvas->Fill(0, 255, 0);

	int center_x = canvas->width() / 2;
	int center_y = canvas->height() / 2;
	float radius_max = canvas->width() / 2;
	float angle_step = 1.0 / 360;

	int counter = 0;

	for (float a = 0, r = 0; r < radius_max; a += angle_step, r += angle_step) {
		if (interrupt_received)
			return;
		float dot_x = cos(a * 2 * M_PI) * r;
		float dot_y = sin(a * 2 * M_PI) * r;

		if (counter == 2000) canvas->Fill(0, 0, 255);

		counter++;

		canvas->SetPixel(center_x + dot_x, center_y + dot_y,
									255, 0, 0);
		usleep(1 * 1000);  // wait a little to slow down things.
	}*/

	Vector2 *pos = new Vector2 {0, 0};

	moveTo(pos, {canvas->width(), canvas->height()}, canvas);
	moveTo(pos, {0, 0}, canvas);

	/*for (int y = 0; y < canvas->height(); y += 1)
	{
		if (y > 4) {
			canvas->SetPixel(centerX, y - 4,
				0, 0, 0);
			canvas->SetPixel(centerX-1, y - 4,
				0, 0, 0);
		}
		canvas->SetPixel(centerX, y,
			255, 0, 0);
		canvas->SetPixel(centerX-1, y,
			255, 0, 0);
		usleep(10000); 
	}*/
}

int main(int argc, char *argv[])
{

	RGBMatrix::Options options;
	options.hardware_mapping = "adafruit-hat-pwm";
	options.rows = 64;
	options.cols = 64;
	options.chain_length = 2;
	options.parallel = 1;
	options.show_refresh_rate = true;
	options.pwm_lsb_nanoseconds = 70;
	Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &options);

	if (canvas == nullptr) return 1;

	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);

	draw(canvas);

	canvas->Clear();
	delete canvas;

	return 0;
}

