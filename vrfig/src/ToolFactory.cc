// $Id: ToolFactory.cc,v 1.10 2001-06-10 19:13:10 jle Exp $

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

#include "ToolFactory.hpp"
#include "tools/MoveFigTool.hpp"
#include "tools/PolyLineTool.hpp"
#include "tools/MoveTool.hpp"
#include "tools/DeleteTool.hpp"
#include "tools/EditTool.hpp"
#include "tools/RectangleTool.hpp"
#include "tools/EllipseTool.hpp"
#include "tools/LabelTool.hpp"

vector<Tool *> *ToolFactory::create_tools() {
  vector<Tool *> *tools = new vector<Tool *>();
  tools->insert(tools->end(), new PolyLineTool());
  tools->insert(tools->end(), new RectangleTool());
  tools->insert(tools->end(), new EllipseTool());
  tools->insert(tools->end(), new LabelTool());
  tools->insert(tools->end(), new MoveTool());
  tools->insert(tools->end(), new EditTool());
  tools->insert(tools->end(), new DeleteTool());
  tools->insert(tools->end(), new MoveFigTool());
  return tools;
}
