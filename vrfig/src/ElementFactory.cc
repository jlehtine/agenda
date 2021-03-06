// $Id: ElementFactory.cc,v 1.9 2001-05-25 22:31:18 jle Exp $

/*--------------------------------------------------------------------------
 * VRFig, a vector graphics editor for PDA environment
 * Copyright 2001 Johannes Lehtinen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *------------------------------------------------------------------------*/

#include <map.h>
#include <string>
#include <string.h>
#include <FL/fl_ask.H>
#include "ElementFactory.hpp"
#include "elements/PolyLine.hpp"
#include "elements/Rectangle.hpp"
#include "elements/Ellipse.hpp"
#include "elements/Label.hpp"

#define PARSER_BUFFER_SIZE 1024

struct char_ptr_less {
  inline bool operator()(const char *s1, const char *s2) const {
    if (!s1 && s2)
      return true;
    else if (s1 && s2 && strcmp(s1, s2) < 0)
      return true;
    else
      return false;
  }
};

/** Mapping from element names to deserialization functions */
static map<const char *, void (*)(XML_Parser, ElementFactory *), 
  char_ptr_less> *deserializers = 0;

// Initializes the list of deserializers
static void init_deserializers();

static void add_deserializer(const char *ns, const char *name, 
                             element_deserializer deserializer);

Figure *ElementFactory::deserialize(istream &is) {

  // Initialize list of deserializers if necessary
  if (!deserializers)
    init_deserializers();

  // Create a new figure
  Figure *figure = new Figure();
  
  // Create element factory and parse the document
  ElementFactory *ef = new ElementFactory(figure, is);
  if (!ef->parse()) {
    delete ef;
    delete figure;
    return 0;
  }

  // Return the result
  delete ef;
  return figure;
}

ElementFactory::ElementFactory(Figure *figure, istream &is): 
  figure(figure), input_stream(is), depth(0), parsing_elements(false),
  error(false) {
  parser = XML_ParserCreateNS("UTF-8", '#');
}

ElementFactory::~ElementFactory() {
  XML_ParserFree(parser);
}

bool ElementFactory::parse() {
  
  // Set up XML handlers
  XML_SetElementHandler(parser, start_handler_static, end_handler_static);
  XML_SetUserData(parser, this);

  // Feed input to XML parser
  while (1) {
    void *parser_buffer = XML_GetBuffer(parser, PARSER_BUFFER_SIZE);
    input_stream.read(parser_buffer, PARSER_BUFFER_SIZE);
    if (input_stream.bad()) {
      fl_alert("Error when reading input stream.");
      return false;
    } else {
      if (!XML_ParseBuffer(
        parser, input_stream.gcount(), input_stream.eof())) {
        fl_alert("Document is malformed. XML parser error occurred on "
                 "line %d: %s", 
                 XML_GetCurrentLineNumber(parser),
                 XML_ErrorString(XML_GetErrorCode(parser)));
        return false;
      }
    }
    if (error)
      return false;
    if (input_stream.eof())
      return true;
  }
}

void ElementFactory::start_handler_static(
  void *data, const XML_Char *name, const XML_Char **atts) {
  reinterpret_cast<ElementFactory *>(data) -> start_handler(name, atts);
}

void ElementFactory::start_handler(
  const XML_Char *name, const XML_Char **atts) {
  static const char *figure_name = VRF_DEFAULT_NAMESPACE "#figure";
  static const char *elements_name = VRF_DEFAULT_NAMESPACE "#elements";

  // Check root tag
  if (depth == 0) {
    if (strcmp(name, figure_name))
      msg_error("The input file is not a VRFig data file.");
  }

  // Check if just arrived to elements
  else if (depth == 1) {
    if (!strcmp(name, elements_name))
      parsing_elements = true;
  }

  // Parse the next element if in elements
  else if (depth == 2 && parsing_elements) {
    element_deserializer ed = (*deserializers)[name];
    if (!ed) {
      const char *sn = strchr(name, '#');
      if (!sn)
        sn = name;
      else
        sn++;
      fl_alert("Skipping unknown element %s.", sn);
    } else {
      
      // Unregister default handlers
      XML_SetElementHandler(parser, 0, 0);
      ed(parser, this);
    }
  }
  depth++;
}

void ElementFactory::end_handler_static(void *data, const XML_Char *name) {
  reinterpret_cast<ElementFactory *>(data) -> end_handler(name);
}

void ElementFactory::end_handler(const XML_Char *name) {
  if (depth == 2)
    parsing_elements = false;
  depth--;
}

void ElementFactory::done(Element *element) {
  depth--;
  XML_SetElementHandler(parser, start_handler_static, end_handler_static);
  if (element)
    figure->add_element(element);
}

void ElementFactory::msg_warning(char *msg) {
  fl_alert(msg);
}

void ElementFactory::msg_error(char *msg = 0) {
  if (msg)
    fl_alert(msg);
  else
    fl_alert("Error when parsing the input file.");
  error = true;
  XML_SetElementHandler(parser, 0, 0);
}

static void init_deserializers() {
  deserializers =
    new map<const char *, void (*)(XML_Parser, ElementFactory *),
    char_ptr_less>;

  add_deserializer(PolyLine::get_namespace_static(),
                   PolyLine::get_name_static(),
                   PolyLine::deserialize);
  add_deserializer(Rectangle::get_namespace_static(),
                   Rectangle::get_name_static(),
                   Rectangle::deserialize);
  add_deserializer(Ellipse::get_namespace_static(),
                   Ellipse::get_name_static(),
                   Ellipse::deserialize);
  add_deserializer(Label::get_namespace_static(),
                   Label::get_name_static(),
                   Label::deserialize);
}

static void add_deserializer(const char *ns, const char *name, 
                             element_deserializer deserializer) {
  string str(ns);
  str.append("#");
  str.append(name);
  char *cp = new char[str.length() + 1];
  strcpy(cp, str.c_str());
  (*deserializers)[cp] = deserializer;
}
