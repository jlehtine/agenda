// $Id: ToolFactory.cc,v 1.1 2001-05-17 19:35:28 jle Exp $

#include "ToolFactory.hpp"
#include "tools/DrawingTool.hpp"
#include "tools/MoveFigTool.hpp"

vector<Tool *> *ToolFactory::create_tools() {
  vector<Tool *> *tools = new vector<Tool *>();
#if USE_EXPERIMENTAL_UI
  tools->insert(tools->end(), new DrawingTool());
#endif
  tools->insert(tools->end(), new MoveFigTool());
  return tools;
}
