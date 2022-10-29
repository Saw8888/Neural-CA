#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <GL/glut.h>

#define sleep(x) Sleep(1000 * (x))

#define sizeX 1000
#define sizeY 500
#define wallSize 1

double grid[sizeY][sizeX];
double newGrid[sizeY][sizeX];
double filter[3][3] = {{0.68, -0.9,  0.68},
                       {-0.9, -0.66, -0.9},
                       {0.68, -0.9,   0.68}};
                       

int width = 1000;
int height = 500;

double activation(double x){
	return -1/pow(2, (0.6*pow(x, 2)))+1;
}

void CVFilter(){
	int x,y;
	double cv; //covolved value
	for(y=1;y<sizeY-1;y++){
  for(x=1;x<sizeX-1;x++){
  	cv = (grid[y][x]*filter[1][1]) + (grid[y][x-1]*filter[1][0]) + (grid[y][x+1]*filter[1][2])+
			     (grid[y+1][x]*filter[2][1]) + (grid[y+1][x-1]*filter[2][0]) + (grid[y+1][x+1]*filter[2][2])+
			     (grid[y-1][x]*filter[0][1]) + (grid[y-1][x-1]*filter[0][0]) + (grid[y-1][x+1]*filter[0][2]);
			newGrid[y][x] = activation(cv);
  }
 }
}

void drawGrid(){
 int x,y;
 double rgb;
 glPointSize(wallSize);
 glBegin(GL_POINTS);
 for(y=0;y<sizeY;y++){
  for(x=0;x<sizeX;x++){
  	rgb = grid[y][x];
  	glColor3f(rgb,0,0);
  	glVertex2i((x*wallSize),(y*wallSize));
  }
 }
 glEnd();
}

int randRange(int min, int max){
 return min + (int) (rand() / (double) (RAND_MAX + 1) * (max - min + 1));
}

float randFloat(){
 float scale=RAND_MAX+1.;
 float base=rand()/scale;
 float fine=rand()/scale;
 return base+fine/scale;
}

void fillGrid(){
	int x,y;
	for(y=0;y<sizeY;y++){
  for(x=0;x<sizeX;x++){
  	grid[y][x] = randRange(-1,1);
  }
 }
}

void cloneGrids(){
	int x,y;
	for(y=0;y<sizeY;y++){
  for(x=0;x<sizeX;x++){
  	grid[y][x] = newGrid[y][x];
  }
 }
}

void display(){
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 //---------------------Proccess--------------------
 CVFilter();
 cloneGrids();
 //-----------------------Draw----------------------
 drawGrid();
 //-------------------------------------------------
 glutSwapBuffers();
 glutPostRedisplay();
}

void init(){
 glClearColor(0.3,0.3,0.3,0);
 gluOrtho2D(0,width,height,0);
}

void main(int argc, char** argv){ 
 fillGrid();
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 glutInitWindowSize(width,height);
 glutCreateWindow("OpenGL");
 init();
 glutDisplayFunc(display);
 glutMainLoop();
}
