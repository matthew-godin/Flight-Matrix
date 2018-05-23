#include "latLongConv.hpp"
#include "led-matrix.h"
#include <stdio.h>
#include <signal.h>
#include <vector>
#include <time.h>
#include "graphics.h"
#include <cassert>
#include <unistd.h>

using namespace std;
using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

time_t t = time(NULL);
struct tm tme = *localtime(&t);

//interrupt and interrupt handling setup
volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
	interrupt_received = true;
}

static short findSaturation(int latitude){
	tme.tm_hour

}

static void drawMap(Canvas *canvas) {
	short sat = 255;
	short blue[3] = {0,0,sat};
	short green[3] = {0,sat,0};

	for(int i = 0; i < 32; i++){
		sat = findSaturation();
		blue[2] = sat;
		green[1] = sat;

		for(int j = 0; j < 64; j++){
			if(initMap[i][j] == 1){
				canvas->SetPixel(j,i,green[0],green[1],green[2]);
			}else{
				canvas->SetPixel(j,i,blue[0],blue[1],blue[2]);
			}
			
		}
	}
}

static void drawPlanes(Canvas *canvas,std::vector<matCoord> planes){
	short numPlanes = planes.size();
	for(int i = 0; i < numPlanes; i++){
		canvas->SetPixel(planes.at(i).x,planes.at(i).y,255,0,0);
	}
}

static void drawTime(Canvas* canvas){

	RGBMatrix::Options matrix_options;
	rgb_matrix::RuntimeOptions runtime_opt;

	rgb_matrix::Font font;

	const char *bdf_font_file = "../fonts/5x8.bdf";
	font.LoadFont(bdf_font_file);
	rgb_matrix::Color bg_color(0, 0, 180);
	rgb_matrix::Color color(255, 255, 255);

	char x_orig = 1;
	char y_orig = 16;
	char letter_spacing = 0;

	char x = x_orig;
	char y = y_orig;

	char time_arr1[3];
	char time_arr2[3];

	time_arr1[0] = (char) ((tme.tm_hour-4 < 0)?24-(tme.tm_hour-4):tme.tm_hour-4)/10 + 48;
	time_arr1[1] = (char) ((tme.tm_hour-4 < 0)?24-(tme.tm_hour-4):tme.tm_hour-4)%10 + 48;
	time_arr1[2] = ':';
	time_arr2[0] = (char) tme.tm_min/10 + 48;
	time_arr2[1] = (char) tme.tm_min%10 + 48;
	time_arr2[2] = '\0';

	rgb_matrix::DrawText(canvas, font, x, y + font.baseline(),color, &bg_color ,time_arr1,-1);
	
	x += 3;
	y += font.height();
	
	rgb_matrix::DrawText(canvas, font, x, y + font.baseline(),color, &bg_color ,time_arr2,-1);


}

static void drawAirports(Canvas *canvas,vector<matCoord> &airports){
	short numAirports = airports.size();
	for(int i = 0; i < numAirports; i++){
		canvas->SetPixel(airports.at(i).x,airports.at(i).y,0,0,0);
	}
}

static void addAirports(vector<matCoord> &airports){
	matCoord mat;
	mat.x = 8;mat.y = 13;airports.push_back(mat);
	mat.x = 14;mat.y = 11;airports.push_back(mat);
	mat.x = 15;mat.y = 13;airports.push_back(mat);
	mat.x = 18;mat.y = 11;airports.push_back(mat);
	mat.x = 58;mat.y = 13;airports.push_back(mat);
	mat.x = 12;mat.y = 13;airports.push_back(mat);
	mat.x = 34;mat.y = 9;airports.push_back(mat);
	mat.x = 16;mat.y = 13;airports.push_back(mat);
	mat.x = 35;mat.y = 9;airports.push_back(mat);
	mat.x = 54;mat.y = 15;airports.push_back(mat);
	mat.x = 32;mat.y = 12;airports.push_back(mat);
	mat.x = 33;mat.y = 15;airports.push_back(mat);
	mat.x = 35;mat.y = 16;airports.push_back(mat);
	mat.x = 16;mat.y = 13;airports.push_back(mat);
	mat.x = 35;mat.y = 9;airports.push_back(mat);
	mat.x = 52;mat.y = 15;airports.push_back(mat);
	mat.x = 50;mat.y = 18;airports.push_back(mat);
	mat.x = 56;mat.y = 14;airports.push_back(mat);
	mat.x = 37;mat.y = 14;airports.push_back(mat);
	mat.x = 42;mat.y = 12;airports.push_back(mat);
	mat.x = 56;mat.y = 21;airports.push_back(mat);
	mat.x = 47;mat.y = 14;airports.push_back(mat);
}

int main(int argc, char *argv[]) {
	RGBMatrix::Options defaults;
	vector<matCoord> airports, planes;
	addAirports(airports);
	defaults.hardware_mapping = "adafruit-hat";
	defaults.rows = 32;
	defaults.cols = 64;
	defaults.chain_length = 1;
	defaults.parallel = 1;
	defaults.brightness=40;
	Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
	if (canvas == NULL) {
		return 1;
	}

	//exit when CTRL-C
	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);

	while(!interrupt_received){
		previousRefresh = toRefresh;
		drawMap(canvas);
		drawTime(canvas);
		getPlaneCoord(planes);
		drawPlanes(canvas,planes);
		planes.clear();
		usleep(60000000);
	}
	
	//clear and delete the canvas if interrupt is sent
	canvas -> Clear();
	delete canvas;

	return 0;
};

