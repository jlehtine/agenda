// $Id: ToolFactory.cc,v 1.7 2001-05-23 07:55:11 jle Exp $

#include "ToolFactory.hpp"
#include "tools/DrawingTool.hpp"
#include "tools/MoveFigTool.hpp"
#include "tools/PolyLineTool.hpp"
#include "tools/MoveTool.hpp"
#include "tools/DeleteTool.hpp"
#include "tools/EditTool.hpp"
#include "tools/RectangleTool.hpp"
#include "tools/EllipseTool.hpp"

vector<Tool *> *ToolFactory::create_tools() {
  vector<Tool *> *tools = new vector<Tool *>();
#if USE_EXPERIMENTAL_UI
  tools->insert(tools->end(), new DrawingTool());
#endif
  tools->insert(tools->end(), new PolyLineTool());
  tools->insert(tools->end(), new RectangleTool());
  tools->insert(tools->end(), new EllipseTool());
  tools->insert(tools->end(), new MoveTool());
  tools->insert(tools->end(), new EditTool());
  tools->insert(tools->end(), new DeleteTool());
  tools->insert(tools->end(), new MoveFigTool());
  return tools;
}
