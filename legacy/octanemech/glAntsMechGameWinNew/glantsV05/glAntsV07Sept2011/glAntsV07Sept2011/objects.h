//
// Berlin Brown
// bigbinc@hotmail.com
// 
// objects.h
// driver for object source
//
#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#define CUBE_OBJECT			0
#define GRID_OBJECT			1
#define ANT_OBJECT			2
#define WORLD_OBJECT		3
#define WIREBOX_OBJECT		4
#define NORM_CUBE_OBJECT	5
#define SQUARE_OBJECT		6
#define PLANE_OBJECT		7
#define FIREANT_OBJECT		8
#define PYRAMID_OBJECT		9
#define STARS_OBJECT		10

#define MAX_OBJECTS			11

#define GRID_SIZE			300.0f

extern GLUquadricObj *quadric;	// main Quadric Object

//
// major struct for driver objects
//
typedef struct tagDriverObjects {

	void (*init)(int list_id);
	void (*compile)(void);
	void (*draw)(void);		// used with compile
	void (*render)(void);	// render object to scenes

	int	call_id;			// id used to compile object
	int visible;

} DriverObjects;

void InitObjects(void);
void DeleteObjects(void);
void RenderObjects(void);
void RenderGrid(void);

void RenderWalls(void);

void RenderBounds(float x, float y, float width);

void Bot_Triangle(float x, float y);

//
// set of the objects
//
extern DriverObjects	colorcube;
extern DriverObjects	grid;
extern DriverObjects	ant;
extern DriverObjects	walls;
extern DriverObjects	wirebox;
extern DriverObjects	norm_cube;
extern DriverObjects	pheromone;
extern DriverObjects	plane;
extern DriverObjects	fireant;
extern DriverObjects	pyramid;
extern DriverObjects	stars;

extern DriverObjects *driver_objects[MAX_OBJECTS];

void RenderPlane(void);
#endif
