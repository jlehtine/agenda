// $Id: ToolFactory.hpp,v 1.2 2001-05-23 12:47:51 jle Exp $

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

#ifndef __ToolFactory_hpp_INCLUDED__
#define __ToolFactory_hpp_INCLUDED__

#include "Tool.hpp"

/**
 * This class constructs a list of available tools. In the future it would be
 * possible to replace this factory with a plug-in mechanism where the
 * tools would be loaded from one or more shared object files.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
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
