// $Id: Rectangle.hpp,v 1.1 2001-05-22 21:07:34 jle Exp $

#ifndef __Rectangle_hpp_INCLUDED__
#define __Rectangle_hpp_INCLUDED__

#include "Element.hpp"
#include "Movable.hpp"
#include "Selectable.hpp"
#include "Controllable.hpp"

class Rectangle : public Element, public Selectable, public Movable,
                  public Controllable {

protected:

  /** The x coordinate of the lower left corner */
  fp16 x;

  /** The y coordinate of the lower left corner */
  fp16 y;

  /** The width */
  fp16 w;
  
  /** The height */
  fp16 h;
  
  /** The control points (redundant information updated based on x, y, w, h) */
  vector<fp16> controls;

  /**
   * Updates the control points based on the current x, y, w, h values.
   */
  void update_controls();

public:

  /**
   * Constructs a new rectangle.
   *
   * @param x the x coordinate of the lower left corner
   * @param y the y coordinate of the lower left corner
   * @param w the width of the rectangle
   * @param h the height of the rectangle
   */
  inline Rectangle(fp16 x, fp16 y, fp16 w, fp16 h): 
    x(x), y(y), w(w), h(h), controls(8) {
    update_controls();
  }

  virtual const char *get_name() const;
  
  virtual const char *get_namespace() const;
  
  virtual void get_bounding_box(fp16 &x, fp16 &y, fp16 &w, fp16 &h) 
    const;
  
  virtual void draw(fp16 origin_x, fp16 origin_y, u_fp16 scaling,
                    bool xorred=false) const;
  
  virtual ostream &serialize(ostream &os, const char *ns, int indent) 
    const;
  
  virtual u_fp32 select_distance_sqr(fp16 x, fp16 y) const;
  
  virtual void move(fp16 xoff, fp16 yoff);
  
  virtual const vector<fp16> *get_control_points() const;

  virtual void control(unsigned int i, fp16 x, fp16 y);
  
};

#endif
