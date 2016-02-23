#if !defined(BITMAP_HH)
#define BITMAP_HH

#include <atomic>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "vec.hh"

class bitmap {
public:
  // Constructor: set up the bitmap width, height, and data array
  bitmap(size_t width, size_t height) : _width(width), _height(height) {
    _data = new rgb32[width*height];
  }
  
  // Destructor: free the data array
  ~bitmap() {
    delete _data;
  }
  
  // Get the size of this bitmap's image data
  size_t size() { return _width*_height*sizeof(rgb32); }
  
  // Copy this bitmap to a given data location
  void copy_to(void* dest) {
    memcpy(dest, _data, size());
  }
  
  // Disallow the copy constructor for bitmaps
  bitmap(const bitmap&) = delete;
  bitmap(bitmap&&) = delete;
  
  // Disallow copying assignment for bitmaps
  bitmap& operator=(const bitmap&) = delete;
  bitmap& operator=(bitmap&&) = delete;
  
  // Get the height of the bitmap
  size_t height() { return _height; }
  
  // Get the width of the bitmap
  size_t width() { return _width; }
  
  // Set the color at a given location
  void set(size_t x, size_t y, vec c) {
    assert(x < _width && "X coordinate is out of bounds");
    assert(y < _height && "Y coordinate is out of bounds");
    _data[y*_width+x] = { 0, (uint8_t)fmax(0, fmin(255, 255*c.x())),
                             (uint8_t)fmax(0, fmin(255, 255*c.y())),
                             (uint8_t)fmax(0, fmin(255, 255*c.z())) };
  }
  
private:
  size_t _width;
  size_t _height;
  
  struct rgb32 {
    uint8_t alpha;
    uint8_t blue;
    uint8_t green;
    uint8_t red;
  };
  
  rgb32* _data;
};

#endif
