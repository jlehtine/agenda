// $Id: Editor.hpp,v 1.3 2001-05-22 14:53:27 jle Exp $

#ifndef __Editor_hpp_INCLUDED__
#define __Editor_hpp_INCLUDED__

#include "FigureView.hpp"
#include "Tool.hpp"

/**
 * The widget used for the editing area. This class is responsible for
 * rendering the elements, passing user activities to tools and calling
 * the tool specific drawing hooks.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.3 $
 */
class Editor : public FigureView {

protected:

  /** The current tool */
  Tool *tool;

public:

  /**
   * Constructs a new editor and initializes it for default look. No
   * figure or callback hooks are associated with the editor.
   */
  Editor(int x, int y, int w, int h);

  virtual void set_figure(Figure *figure);

  /**
   * Sets the current tool.
   *
   * @param tool the new tool to be used
   */
  void set_tool(Tool *_tool);

  void set_origin(fp16 x, fp16 y);

  void set_scaling(u_fp16 _scaling);

  /**
   * Draws the widget.
   */
  void draw();

  /**
   * Handle the events.
   */
  int handle(int event);
};

#endif
