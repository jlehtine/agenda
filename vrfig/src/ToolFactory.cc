// $Id: ToolFactory.cc,v 1.2 2001-05-19 16:57:15 jle Exp $

#include "ToolFactory.hpp"
#include "tools/DrawingTool.hpp"
#include "tools/MoveFigTool.hpp"
#include "tools/PolyLineTool.hpp"

vector<Tool *> *ToolFactory::create_tools() {
  vector<Tool *> *tools = new vector<Tool *>();
#if USE_EXPERIMENTAL_UI
  tools->insert(tools->end(), new DrawingTool());
#endif
  tools->insert(tools->end(), new MoveFigTool());
  tools->insert(tools->end(), new PolyLineTool());
  return tools;
}
