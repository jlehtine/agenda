// $Id: ToolFactory.hpp,v 1.1 2001-05-17 19:35:28 jle Exp $

#ifndef __ToolFactory_hpp_INCLUDED__
#define __ToolFactory_hpp_INCLUDED__

#include "Tool.hpp"

/**
 * This class constructs a list of available tools. In the future it would be
 * possible to replace this factory with a plug-in mechanism where the
 * tools would be loaded from one or more shared object files.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class ToolFactory {

public:

  /**
   * Constructs instances of the available tools and returns them
   * as a vector.
   *
   * @return the tools available
   */
  static vector<Tool *> *create_tools();
};

#endif
