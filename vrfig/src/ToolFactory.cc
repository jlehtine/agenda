// $Id: ToolFactory.cc,v 1.3 2001-05-22 18:04:44 jle Exp $

#include "ToolFactory.hpp"
#include "tools/DrawingTool.hpp"
#include "tools/MoveFigTool.hpp"
#include "tools/PolyLineTool.hpp"
#include "tools/MoveTool.hpp"

vector<Tool *> *ToolFactory::create_tools() {
  vector<Tool *> *tools = new vector<Tool *>();
#if USE_EXPERIMENTAL_UI
  tools->insert(tools->end(), new DrawingTool());
#endif
  tools->insert(tools->end(), new PolyLineTool());
  tools->insert(tools->end(), new MoveTool());
  tools->insert(tools->end(), new MoveFigTool());
  return tools;
}
