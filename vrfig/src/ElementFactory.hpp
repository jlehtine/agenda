// $Id: ElementFactory.hpp,v 1.1 2001-05-21 00:47:36 jle Exp $

#ifndef __ElementFactory_hpp_INCLUDED__
#define __ElementFactory_hpp_INCLUDED__

#include <iostream.h>
#include <expat.h>
#include "Figure.hpp"

/**
 * Class for deserializing XML format element data to element objects.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@ik.fi>
 * @version $Revision: 1.1 $
 */
class ElementFactory {

protected:

  /** The figure being deserialized */
  Figure *figure;

  /** The XML parser used for parsing */
  XML_Parser parser;

  /** The buffer used for reading data */
  void *parser_buffer;

  /** The input stream for data */
  istream &input_stream;

  /** The current parsing depth */
  int depth;

  /** Whether currently in the elements section */
  bool parsing_elements;

  /** Whether there has been a fatal error */
  bool error;

  /**
   * Constructs a new factory.
   */
  ElementFactory(Figure *figure, istream &is);

  ~ElementFactory();

  /**
   * Parses the document.
   *
   * @return whether the parsing succeeded or not
   */
  bool parse();

  /**
   * Handler for starting elements.
   */
  static void start_handler_static(void *data, const XML_Char *name,
                                   const XML_Char **atts);
  void start_handler(const XML_Char *name, const XML_Char **atts);

  /**
   * Handler for ending elements.
   */
  static void end_handler_static(void *data, const XML_Char *name);
  void end_handler(const XML_Char *name);
  
public:

  /**
   * Deserializes the specified input to a figure. Returns 0-pointer if
   * there were fatal parsing errors. Displays all warnings directly
   * to the user.
   *
   * @param is the input stream for XML data
   */
  static Figure *deserialize(istream &is);

  /**
   * Returns the figure being deserialized.
   *
   * @return the figure being deserialized
   */
  inline Figure *get_figure() {
    return figure;
  }

  /**
   * An element specific deserializer must call this method to acknowledge
   * that it has finished parsing of an element. Before calling this
   * method the deserializer should unset any XML parser handlers it has
   * used.
   */
  void done();

  /**
   * An element specific deserializer can call this method to warn
   * about non-fatal issues during deserialization.
   *
   * @param msg the warning message
   */
  void msg_warning(char *msg);

  /**
   * An element specific deserializer can call this method to signal
   * a parsing error. Before calling this method the deserializer should
   * unset any XML parser handlers it has used.
   *
   * @param msg the error message
   */
  void msg_error(char *msg = 0);
};

/**
 * An element specific deserializer. These deserializers should set up
 * any possible XML parser handlers they need and consume the element
 * data until they find the closing tag for the enclosing XML element.
 * After finishing the deserializer should unset any handlers it used
 * and call the done() method of the calling ElementFactory. If the
 * element deserializer wishes to use the XML parser user data pointer,
 * it should store the old value and restore it before finishing.
 *
 * @param parser the XML parser
 * @param ef the calling element factory
 */
typedef void (*element_deserializer)(XML_Parser parser, ElementFactory *ef);

#endif
