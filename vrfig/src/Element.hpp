// $Id: Element.hpp,v 1.4 2001-05-16 19:50:16 jle Exp $

#ifndef __Element_hpp_INCLUDED__
#define __Element_hpp_INCLUDED__

#include <string>
#include <vector.h>
#include <ostream.h>

/**
 * Figure is composed of elements such as lines or circles. The elements
 * can be created and edited using the tools. Elements take care of
 * rendering themselves and each element also includes static code to
 * determine if a specific set of points could be matched by the
 * element. This is the common base class for all elements.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 */
class Element {

protected:

  /**
   * Writes an element name with an optional namespace identifier to
   * the specified output stream.
   *
   * @param os the output stream
   * @param ns the namespace identifier or null if default namespace
   * @param name the name of the element
   * @return the output stream for continuation
   */
  static ostream &output_ns_name(ostream &os, const string *ns, 
                                 const string *name) {
    if (ns)
      os << (*ns) << ":";
    os << (*name);
    return os;
  }

  /**
   * Output indentation to the specified output stream.
   *
   * @param os the output stream
   * @param indent the depth of indentation
   * @return the output stream for continuation
   */
  static ostream &output_indent(ostream &os, int indent) {
    while (indent--)
      os << " ";
    return os;
  }

public:

  /**
   * Returns the name of this element (like "polyline" or "circle").
   *
   * @return the name of this element
   */
  virtual const string *get_name() const = 0;

  /**
   * Returns the XML namespace for this element. The namespace is
   * used when serializing/deserializing data to/from XML document.
   * This could be something like "http://xyz.zy/author/vrfig/myelements".
   *
   * @return the namespace for this element.
   */
  virtual const string *get_namespace() const = 0;

  /**
   * Returns the bounding box for the element.
   *
   * @param x the coordinate of the left edge of the box (16.16 fixed)
   * @param y the coordinate of the bottom edge of the box (16.16 fixed)
   * @param w the width of the box (16.16 fixed)
   * @param h the height of the box (16.16 fixed)
   */
  virtual void get_bounding_box(int &x, int &y, int &w, int &h) 
    const = 0;

  /**
   * Draws the element to the current widget using the specified scaling
   * and origin.
   *
   * @param origin_x the x coordinate for the origin (16.16 fixed)
   * @param origin_y the y coordinate for the origin (16.16 fixed)
   * @param scaling the scaling used (16.16 fixed)
   * @param xor whether the element should be drawn xorred
   */
  virtual void draw(int origin_x, int origin_y, unsigned int scaling,
                    bool xorred=false) const = 0;
  
  /**
   * Calculate the distance between the specified point and this
   * element.
   *
   * @param x the x coordinate of the point (16.16 fixed)
   * @param y the y coordinate of the point (16.16 fixed)
   * @return the distance (16.16)
   */
  virtual int distance_to_point(int x, int y) = 0;

  /**
   * Serializes element data to the specified ostream. The data is serialized
   * as an XML data using UTF-8 charset. The caller creates an enclosing
   * XML element so the serialized data does not need to be within one
   * element.
   *
   * @param os the output stream for element data
   * @param ns the namespace identifier for the element namespace or \
   *        null if the default namespace should be used
   * @param indent the base indentation level
   * @return the output stream for continuation
   */
  virtual ostream &serialize(ostream &os, const string *ns, int indent) 
    const = 0;

#if USE_EXPERIMENTAL_UI
  /**
   * Fits some element to the specified point set or returns
   * null if no element could be fitted.
   *
   * @param points a vector of point x/y coordinates
   * @param origin_x the x coordinate for the origin (16.16 fixed)
   * @param origin_y the y coordinate for the origin (16.16 fixed)
   * @param scaling the scaling used (16.16 fixed)
   * @return the fitted element
   */
  static Element *fit_element(
    vector<int> *points, int origin_x, int origin_y, unsigned int scaling);
#endif
};

#endif
