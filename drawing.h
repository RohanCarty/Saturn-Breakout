#include <yaul.h>

int initDrawing(); // Initialises VDP1/VDP2 drawing while 
// setting resolution and output as well as loading required textures

int updateDrawing(); // Process all the queued drawing tasks and then sync to the screen