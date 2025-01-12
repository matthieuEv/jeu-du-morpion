/**
  *   @file window.h
  * 
  *   @author Guitter Julien
  *   @date 1 jan 2025
  */

#ifndef WINDOW_H
#define WINDOW_H

void init_window(int width, int height, bool resizable, char* title);
void set_mouse_callback(void (*callback)(double, double));

void free_window();
int close_window();
int windowShouldClose();

unsigned int add_image(const char* texture_path, double width, double height);
int* draw_image(unsigned int object, double x, double y);
void clear_image(int* object);
void clear_all_images();
void render_window();

void get_window_size(int* width, int* height);

#endif // WINDOW_H