#if !defined(GEOM_HH)
#define GEOM_HH

#include "bitmap.hh"
#include "vec.hh"

/**
 * An abstract class that represents a shape in the scene
 */
class shape {
public:
  /**
   * Create a new shape
   * \param pos The position of the shape
   */
  shape(vec pos) : _pos(pos), 
                   _reflectivity(0.95),
                   _diffusion(0.75), 
                   _spec_intensity(0.9), 
                   _spec_density(200),
                   _static_color(true) {}
  
  // Getter and setters for the shape's position
  vec get_pos() { return _pos; }
  void set_pos(vec p) { _pos = p; }
  void set_pos(float x, float y, float z) { _pos = vec(x, y, z); }
  
  // Get the color of this shape at a specific point
  vec get_color(vec pos) {
    if(_static_color) return _color;
    else return _color_fn(pos);
  }
  
  // Set this shape to a single static color
  void set_color(vec c) {
    _static_color = true;
    _color = c;
  }
  
  // Supply a function to determine the color of this shape
  void set_color(vec (*color_fn)(vec)) {
    _static_color = false;
    _color_fn = color_fn;
  }
  
  // Getter and setter for reflectivity
  float get_reflectivity() { return _reflectivity; }
  void set_reflectivity(float r) { _reflectivity = r; }
  
  // Getter and setter for the diffusion level
  float get_diffusion() { return _diffusion; }
  void set_diffusion(float d) { _diffusion = d; }
  
  // Getter and setter for the specular highlight intensity
  float get_spec_intensity() { return _spec_intensity; }
  void set_spec_intensity(float s) { _spec_intensity = s; }
  
  // Getter and setter for the specular highlight density
  float get_spec_density() { return _spec_density; }
  void set_spec_density(float s) { _spec_density = s; }
  
  // Abstract method for the intersection calculation (shape-dependent)
  virtual float intersection(vec origin, vec dir) = 0;
  
  // Abstract method to compute a normal at a point (shape-dependent)
  virtual vec normal(vec p) = 0;
    
protected:
  vec _pos;
  vec _color;
  vec (*_color_fn)(vec);  // A function that generates the color at a point
  bool _static_color;     // If true use a fixed color, otherwise use _color_fn
  float _reflectivity;
  float _diffusion;
  float _spec_intensity;
  float _spec_density;
};

/**
 * A sphere at a given position and radius
 */
class sphere : public shape {
public:
  /**
   * Create a sphere at a position with a given radius
   * \param pos     The center of the sphere
   * \param radius  The radius of the sphere
   */
  sphere(vec pos, float radius) : shape(pos), _radius(radius) {}
  
  // Getter and setter for the radius
  float get_radius() { return _radius; }
  void set_radius(float r) { _radius = r; }
  
  // Intersection calculation
  virtual float intersection(vec origin, vec dir) {
    vec v = origin - _pos;
    
    // Next, we solve the quadratic equation that tells us where
    // our viewing angle d intersects with the sphere's surface.
    // First, the piece of the quadratic equation under the sqrt:
    float squared_portion = pow(v.dot(dir), 2.0) - (v.dot(v) - pow(_radius, 2.0));
    
    // If the squared portion is negative, there is no intersection
    if(squared_portion >= 0) {
      // Compute both the plus and minus terms in the quadratic equation
      float t_plus = -v.dot(dir) + sqrt(squared_portion);
      float t_minus = -v.dot(dir) - sqrt(squared_portion);
      
      if(t_plus >= 0 && t_plus < t_minus) {
        return t_plus;
      } else if(t_minus >= 0) {
        return t_minus;
      }
    }
    
    return -1;
  }
  
  // Normal calculation
  virtual vec normal(vec p) {
    return (p - _pos).normalized();
  }
  
private:
  float _radius;
};

/**
 * An infinite plane described by a normal vector and origin
 */
class plane : public shape {
public:
  /**
   * Create a plane
   * \param pos   The origin point of this plane
   * \param norm  A vector that points perpendicular to the face of this plane
   */
  plane(vec pos, vec norm) : shape(pos), _norm(norm) {}
  
  // Getter and setter for the plane normal vector
  vec get_norm() { return _norm; }
  void set_norm(vec n) { _norm = n; }
  
  virtual float intersection(vec origin, vec dir) {
    // Dot the plane normal with the ray direction
    float d_dot_norm = _norm.dot(dir);
    
    // If the dot product is zero, the ray does not intersect
    if(d_dot_norm != 0) {
      float distance = (_pos - origin).dot(_norm) / d_dot_norm;
      if(distance >= 0) return distance;
    }
    
    return -1;
  }
  
  // Get the normal for this plane at a given point (identical everywhere)
  virtual vec normal(vec p) {
    return _norm;
  }
  
private:
  vec _norm;
};

/**
 * A viewport class that maps from pixels on a viewing plane to a ray direction
 */
class viewport {
public:
  /**
   * Create a new viewport
   * \param origin    The position of the viewport's "eye"
   * \param look_dir  A vector that points from the "eye" to the middle of the viewing plane
   * \param up        A vector that points up
   * \param width     The width of the viewing screen
   * \param height    The height of the viewing screen
   */
  viewport(vec origin, vec look_dir, vec up, float width, float height) :
      _origin(origin), _look_dir(look_dir), _width(width), _height(height) {
    
    // Compute a viewing direction unit vector
    vec unit_dir = _look_dir.normalized();
    
    // Make a unit vector perpendicular to the viewing direction.
    _up = (up - unit_dir * up.dot(unit_dir)).normalized();
    
    // Make a unit vector perpendicular to both up and the viewing direction
    _right = _up.rotated_around(unit_dir, M_PI_2).normalized();
    
    // Scale _up and _right
    _up /= fmin(width, height);
    _right /= fmin(width, height);
  }
  
  // Get the origin of the viewpoert
  vec origin() {
    return _origin;
  }
  
  // Get a ray from the origin through a specific viewing plane coordinate
  vec dir(float x, float y) {
    // Compute the point on the viewing plane that we are looking through
    vec p = _look_dir - _right * (x - _width / 2) + _up * (_height / 2 - y);
    
    // Return a unit vector pointing from the view origin to this point
    return p.normalized();
  }
  
private:
  vec _origin;
  vec _look_dir;
  vec _up;
  vec _right;
  float _width;
  float _height;
};

#endif
