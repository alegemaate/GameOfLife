/*
 * Conways Game Of Life
 * Allan Legemaate
 *   1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
 *   2. Any live cell with two or three live neighbours lives on to the next generation.
 *   3. Any live cell with more than three live neighbours dies, as if by over-population.
 *   4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
 * 01/10/2016
 */
#include <allegro.h>
#include <math.h>

#define CELL_SIZE 10

#define LIFE_W 100
#define LIFE_H 100

// Buffer object
BITMAP *buffer;

// Array to store life
int life_array[LIFE_W][LIFE_H];
int save_life_array[LIFE_W][LIFE_H];

// Simulation running
int running = 0;

// Modulo proper
int modulo (int m, int n) {
  return m >= 0 ? m % n : ( n - abs ( m%n ) ) % n;
}

// Random
int random( int smallest, int largest){
  return smallest + ((largest - smallest) * rand()/(RAND_MAX + 1.0));
}

// Copy arrays
void copy_array( int array_1[LIFE_W][LIFE_H], int array_2[LIFE_W][LIFE_H], int width, int height){
  int i, t;
  for( i = 0; i < width; i ++)
    for( t = 0; t < height; t ++)
      array_1[i][t] = array_2[i][t];
}

// Clear arrays
void fill_array( int array_1[LIFE_W][LIFE_H], int width, int height, int value){
  int i, t;
  for( i = 0; i < width; i ++)
    for( t = 0; t < height; t ++)
      array_1[i][t] = value;
}

// Init
void init(){
  // Allegro
  allegro_init();
  install_keyboard();
  install_mouse();

  // Screen
  set_palette(desktop_palette);
  set_color_depth(32);
  set_gfx_mode( GFX_AUTODETECT_WINDOWED, CELL_SIZE * LIFE_W, CELL_SIZE * LIFE_H, 0, 0);

  // Buffer
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Initial fill of life
  fill_array( life_array, LIFE_W, LIFE_H, 0);
}

// Logic
void update(){
  // If running
  if( running){
    // Update
    int i, t;

    // Temp array
    int temp_life_array[LIFE_W][LIFE_H];

    // Init to 0
    fill_array( temp_life_array, LIFE_W, LIFE_H, 0);

    // Simulate!
    for( i = 0; i < LIFE_W; i++){
      for( t = 0; t < LIFE_H; t++){
        // Number of neighbouring cells
        int neighbours = life_array[modulo((i + 1),LIFE_W)][ t                    ] + life_array[modulo((i - 1),LIFE_W)][ t                    ] +
                         life_array[ i                    ][modulo((t + 1),LIFE_H)] + life_array[ i                    ][modulo((t - 1),LIFE_H)] +
                         life_array[modulo((i + 1),LIFE_W)][modulo((t + 1),LIFE_H)] + life_array[modulo((i - 1),LIFE_W)][modulo((t - 1),LIFE_H)] +
                         life_array[modulo((i - 1),LIFE_W)][modulo((t + 1),LIFE_H)] + life_array[modulo((i + 1),LIFE_W)][modulo((t - 1),LIFE_H)];

        // Dies from overpop or underpop
        if( neighbours < 2 || neighbours > 3)
           temp_life_array[i][t] = 0;

        // Is born
        else if( neighbours == 3)
          temp_life_array[i][t] = 1;
        // Stays the same
        else if( neighbours == 2)
          temp_life_array[i][t] = life_array[i][t];
      }
    }
    // Copy array back
    copy_array( life_array, temp_life_array, LIFE_W, LIFE_H);
  }

  // Clear screen
  if( key[KEY_R])
    fill_array( life_array, LIFE_W, LIFE_H, 0);

  // Run/stop
  if( key[KEY_SPACE]){
    running = !running;
    while(key[KEY_SPACE]){}
  }

  // Draw patterns
  if( mouse_b & 1)
    life_array[mouse_x/CELL_SIZE][mouse_y/CELL_SIZE] = 1;
  if( mouse_b & 2)
    life_array[mouse_x/CELL_SIZE][mouse_y/CELL_SIZE] = 0;

  // Save screen for recall
  if( key[KEY_S]){
    fill_array( save_life_array, LIFE_W, LIFE_H, 0);
    copy_array( save_life_array, life_array, LIFE_W, LIFE_H);
  }

  // Load
  if( key[KEY_L]){
    fill_array( life_array, LIFE_W, LIFE_H, 0);
    copy_array( life_array, save_life_array, LIFE_W, LIFE_H);
  }

  // Create a random shape
  if( key[KEY_W]){
    // Random parameters
    int shape_w = random( 1, LIFE_W/4);
    int shape_h = random( 1, LIFE_H/4);

    // Iterators
    int i, t;

    // Make it
    for( i = LIFE_W/2 - shape_w; i < LIFE_W/2 + shape_w; i++){
      for( t = LIFE_H/2 - shape_h; t < LIFE_H/2 + shape_h; t++){
        int new_val = random( 0, 2);
        life_array[i][t] = new_val;
      }
    }

    while(key[KEY_W]){}
  }

  // Create a symetrical shape
  if( key[KEY_Q]){
    // Random parameters
    int shape_w = random( 1, LIFE_W/8);
    int shape_h = shape_w;

    // Iterators
    int i, t;

    // Make it
    for( i = -shape_w; i < 0; i++){
      for( t = -shape_h; t < 0; t++){
        int new_val = random( 0, 2);
        life_array[LIFE_W/2 + i][LIFE_H/2 + t] = new_val;
        life_array[LIFE_W/2 - i][LIFE_H/2 - t] = new_val;
        life_array[LIFE_W/2 + i][LIFE_H/2 - t] = new_val;
        life_array[LIFE_W/2 - i][LIFE_H/2 + t] = new_val;
      }
    }

    while(key[KEY_Q]){}
  }
}

// Drawing
void draw(){
  // Clear screen
  rectfill( buffer, 0, 0, SCREEN_W, SCREEN_H, makecol( 255, 255, 255));

  // Draw array
  int i, t;
  for( i = 0; i < LIFE_W; i++){
    for( t = 0; t < LIFE_H; t++){
      // Count neighbours for heat map
      int neighbours = life_array[modulo((i + 1),LIFE_W)][ t                    ] + life_array[modulo((i - 1),LIFE_W)][ t                    ] +
                       life_array[ i                    ][modulo((t + 1),LIFE_H)] + life_array[ i                    ][modulo((t - 1),LIFE_H)] +
                       life_array[modulo((i + 1),LIFE_W)][modulo((t + 1),LIFE_H)] + life_array[modulo((i - 1),LIFE_W)][modulo((t - 1),LIFE_H)] +
                       life_array[modulo((i - 1),LIFE_W)][modulo((t + 1),LIFE_H)] + life_array[modulo((i + 1),LIFE_W)][modulo((t - 1),LIFE_H)];

      // Draw living shapes
      if( life_array[i][t] == 1){
        rectfill( buffer, i * CELL_SIZE, t * CELL_SIZE, i * CELL_SIZE + CELL_SIZE, t * CELL_SIZE + CELL_SIZE, makecol( (neighbours*31), 0, 0));
        //textprintf_ex( buffer, font, i * CELL_SIZE, t * CELL_SIZE, makecol( 255, 255, 255), -1, "%i", modulo((i - 1),LIFE_W));
      }
    }
  }

  // Mouse
  rectfill( buffer, mouse_x, mouse_y, mouse_x + 5, mouse_y + 5, makecol( 255 * !running, 255 * running, 0));

  // Draw buffer
  draw_sprite( screen, buffer, 0, 0);
}

// Main of program
int main(){
  // Init
  init();

  // Run game
  while( !key[KEY_ESC]){
    update();
    draw();

    // Wait
    if( running)
      rest( 80 * !key[KEY_LCONTROL]);
  }

  // End of program
  return 0;
}
END_OF_MAIN()
