#include "snow.h"

//
// Snow
//

Snow::Snow(Canvas *canvas)
{
	this->canvas = canvas;
	this->snowflakes = {};
}

void Snow::createSnowFlake()
{
	snowflakes.push_back({ 10, 0 });
}

void Snow::updateSnowflakePos()
{
	for (auto snowflake : snowflakes) {
		snowflake.y++;
	}
}

void Snow::render()
{
	updateSnowflakePos();
	for (auto snowflake : snowflakes) {
		canvas->SetPixel(snowflake.x, snowflake.y);
	}
	counter++;
	usleep(10000);
}

void Snow::start()
{
	createSnowFlake();
	while (true)
	{
		this->render();
		if (counter >= 100) break;
	}
}
//
//    Main
//


void draw(Canvas *canvas)
{
	Snow *snow = new Snow(canvas);
	snow->start();
}

void setup(RGBMatrix::Options *options)
{
	options->hardware_mapping = "adafruit-hat-pwm";
	options->rows = 64;
	options->cols = 64;
	options->chain_length = 2;
	options->parallel = 1;
	options->show_refresh_rate = true;
	options->pwm_lsb_nanoseconds = 70;
}

int main(int argc, char *argv[])
{

	RGBMatrix::Options options;
	setup(&options);

	Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &options);

	if (canvas == nullptr) return 1;

	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);

	draw(canvas);

	canvas->Clear();
	delete canvas;

	return 0;
}