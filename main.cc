#include <cmath>
#include <cstdio>
#include <vector>

#include <SDL.h>

#include "bitmap.hh"
#include "geom.hh"
#include "gui.hh"
#include "util.hh"
#include "vec.hh"

// Screen size
#define WIDTH 640
#define HEIGHT 480

// Rendering Properties
#define AMBIENT 0.3         // Ambient illumination

// Set up the 3D scene
void init_scene();

// Trace a ray through the scene to determine its color
vec raytrace(vec origin, vec dir, size_t reflections);

// A list of shapes that make up the 3D scene. Initialized by init_scene
std::vector<shape*> scene;

// A list of light positions, all emitting pure white light
std::vector<vec> lights;

/**
 * Entry point for the raytracer
 * \param argc  The number of command line arguments
 * \param argv  An array of command line arguments
 */
int main(int argc, char** argv) {
  // Create a GUI window
  gui ui("Raytracer", WIDTH, HEIGHT);
  
  // Initialize the 3D scene
  init_scene();
  
  // Set up the viewport
  viewport view(vec(0, 100, -300), // Look from here
                vec(0, -0.25, 1),  // Look in this direction
                vec(0, 1, 0),      // Up is up
                WIDTH,             // Use screen width
                HEIGHT);           // Use screen height
  
  bool running = true;
  
  // Loop until we get a quit event
  while(running) {
    // Process events
    SDL_Event event;
    while(SDL_PollEvent(&event) == 1) {
      // If the event is a quit event, then leave the loop
      if(event.type == SDL_QUIT) running = false;
    }
    
    // Render the frame to this bitmap
    bitmap bmp(WIDTH, HEIGHT);
    
    // Loop over all pixels in the bitmap
    for(int y = 0; y < HEIGHT; y++) {
      for(int x = 0; x < WIDTH; x++) {
        // Get the color of this ray
        vec result = raytrace(view.origin(), view.dir(x, y), 0);

        // Set the pixel color
        bmp.set(x, y, result);
      }
    }
    
    // Display the rendered frame
    ui.display(bmp);
  }
  
  return 0;
}

/**
 * Follow a ray backwards through the scene and return the ray's color
 * \param origin        The origin of the ray
 * \param dir           The direction of the ray
 * \param reflections   The number of times this ray has been reflected
 * \returns             The color of this ray
 */
vec raytrace(vec origin, vec dir, size_t reflections) {
  // Normalize the direction vector
  dir = dir.normalized();
  
  // Keep track of the closest shape that is intersected by this ray
  shape* intersected = NULL;
  float intersect_distance = 0;
  
  // Loop over all shapes in the scene to find the closest intersection
  for(shape* shape : scene) {
    float distance = shape->intersection(origin, dir);
    if(distance >= 0 && (distance < intersect_distance || intersected == NULL)) {
      intersect_distance = distance;
      intersected = shape;
    }
  }
  
  // If the ray didn't intersect anything, just return the ambient color
  if(intersected == NULL) return vec(AMBIENT, AMBIENT, AMBIENT);
  
  // Compute the point where the intersection occurred
  vec intersection = origin + dir * intersect_distance;
  
  // Otherwise just return the color of the object
  return intersected->get_color(intersection);
}

/**
 * Add objects and lights to the scene.
 * Creates three spheres, a flat plane, and two light sources
 */
void init_scene() {
  // Add a red sphere
  sphere* red_sphere = new sphere(vec(60, 50, 0), 50);
  red_sphere->set_color(vec(0.75, 0.125, 0.125));
  red_sphere->set_reflectivity(0.5);
  scene.push_back(red_sphere);
  
  // Add a green sphere
  sphere* green_sphere = new sphere(vec(-15, 25, -25), 25);
  green_sphere->set_color(vec(0.125, 0.6, 0.125));
  green_sphere->set_reflectivity(0.5);
  scene.push_back(green_sphere);
  
  // Add a blue sphere
  sphere* blue_sphere = new sphere(vec(-50, 40, 75), 40);
  blue_sphere->set_color(vec(0.125, 0.125, 0.75));
  blue_sphere->set_reflectivity(0.5);
  scene.push_back(blue_sphere);
  
  // Add a flat surface
  plane* surface = new plane(vec(0, 0, 0), vec(0, 1, 0));
  // The following line uses C++'s lambda expressions to create a function
  surface->set_color([](vec pos) {
    // This function produces a grid pattern on the plane
    if((int)pos.x() % 100 == 0 || (int)pos.z() % 100 == 0) {
      return vec(0.3, 0.3, 0.3);
    } else {
      return vec(0.15, 0.15, 0.15);
    }
  });
  surface->set_diffusion(0.25);
  surface->set_spec_density(10);
  surface->set_spec_intensity(0.1);
  scene.push_back(surface);
  
  // Add two lights
  lights.push_back(vec(-1000, 300, 0));
  lights.push_back(vec(100, 900, 500));
}
