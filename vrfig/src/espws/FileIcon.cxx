//
// "$Id: FileIcon.cxx,v 1.2 2001-05-26 10:13:48 jle Exp $"
//
//   FileIcon routines.
//
//   KDE icon code donated by Maarten De Boer.
//
//   Copyright 1997-2000 by Easy Software Products.
//
//   These coded instructions, statements, and computer programs are the
//   property of Easy Software Products and are protected by Federal
//   copyright law.  Distribution and use rights are outlined in the file
//   "COPYING" which should have been included with this file.  If this
//   file is missing or damaged please contact Easy Software Products
//   at:
//
//       Attn: ESP Licensing Information
//       Easy Software Products
//       44141 Airport View Drive, Suite 204
//       Hollywood, Maryland 20636-3111 USA
//
//       Voice: (301) 373-9600
//       EMail: info@easysw.com
//         WWW: http://www.easysw.com
//
// Modification by Johannes Lehtinen:
//   Splitted to files FileIcon.cxx and FileIcon2.cxx to avoid including
//   unnecessary code when not all the functionality is needed.
//
// Contents:
//
//   FileIcon::FileIcon()          - Create a new file icon.
//   FileIcon::~FileIcon()         - Remove a file icon.
//   FileIcon::add()               - Add data to an icon.
//   FileIcon::find()              - Find an icon based upon a given file.
//   FileIcon::draw()              - Draw an icon.
//   FileIcon::label()             - Set the widgets label to an icon.
//   FileIcon::labeltype()         - Draw the icon label.
//

//
// Include necessary header files...
//

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(WIN32) || defined(__EMX__)
#  include <io.h>
#  define F_OK	0
#  define strcasecmp stricmp
#  define strncasecmp strnicmp
#else
#  include <unistd.h>
#endif /* WIN32 || __EMX__ */

#include "FileIcon.h"

#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/filename.H>


//
// Icon cache...
//

FileIcon	*FileIcon::first_ = (FileIcon *)0;


//
// 'FileIcon::FileIcon()' - Create a new file icon.
//

FileIcon::FileIcon(const char *p,	/* I - Filename pattern */
                   int        t,	/* I - File type */
		   int        nd,	/* I - Number of data values */
		   short      *d)	/* I - Data values */
{
  // Initialize the pattern and type...
  pattern_ = p;
  type_    = t;

  // Copy icon data as needed...
  if (nd)
  {
    num_data_   = nd;
    alloc_data_ = nd + 1;
    data_       = (short *)calloc(sizeof(short), nd + 1);
    memcpy(data_, d, nd * sizeof(short));
  }
  else
  {
    num_data_   = 0;
    alloc_data_ = 0;
  }

  // And add the icon to the list of icons...
  next_  = first_;
  first_ = this;
}


//
// 'FileIcon::~FileIcon()' - Remove a file icon.
//

FileIcon::~FileIcon()
{
  FileIcon	*current,	// Current icon in list
		*prev;		// Previous icon in list


  // Find the icon in the list...
  for (current = first_, prev = (FileIcon *)0;
       current != this && current != (FileIcon *)0;
       prev = current, current = current->next_);

  // Remove the icon from the list as needed...
  if (current)
  {
    if (prev)
      prev->next_ = current->next_;
    else
      first_ = current->next_;
  }

  // Free any memory used...
  if (alloc_data_)
    free(data_);
}


//
// 'FileIcon::add()' - Add data to an icon.
//

short *			// O - Pointer to new data value
FileIcon::add(short d)	// I - Data to add
{
  short	*dptr;		// Pointer to new data value


  // Allocate/reallocate memory as needed
  if ((num_data_ + 1) >= alloc_data_)
  {
    alloc_data_ += 128;

    if (alloc_data_ == 128)
      dptr = (short *)malloc(sizeof(short) * alloc_data_);
    else
      dptr = (short *)realloc(data_, sizeof(short) * alloc_data_);

    if (dptr == NULL)
      return (NULL);

    data_ = dptr;
  }

  // Store the new data value and return
  data_[num_data_++] = d;
  data_[num_data_]   = END;

  return (data_ + num_data_ - 1);
}


//
// 'FileIcon::find()' - Find an icon based upon a given file.
//

FileIcon *				// O - Matching file icon or NULL
FileIcon::find(const char *filename,	// I - Name of file */
               int        filetype)	// I - Enumerated file type
{
  FileIcon	*current;		// Current file in list
  struct stat	fileinfo;		// Information on file


  // Get file information if needed...
  if (filetype == ANY)
    if (!stat(filename, &fileinfo))
    {
      if (S_ISDIR(fileinfo.st_mode))
        filetype = DIRECTORY;
#ifdef S_IFIFO
      else if (S_ISFIFO(fileinfo.st_mode))
        filetype = FIFO;
#endif // S_IFIFO
#if defined(S_ICHR) && defined(S_IBLK)
      else if (S_ISCHR(fileinfo.st_mode) || S_ISBLK(fileinfo.st_mode))
        filetype = DEVICE;
#endif // S_ICHR && S_IBLK
#ifdef S_ILNK
      else if (S_ISLNK(fileinfo.st_mode))
        filetype = LINK;
#endif // S_ILNK
      else
        filetype = PLAIN;
    }

  // Loop through the available file types and return any match that
  // is found...
  for (current = first_; current != (FileIcon *)0; current = current->next_)
    if ((current->type_ == filetype || current->type_ == ANY) &&
        filename_match(filename, current->pattern_))
      break;

  // Return the match (if any)...
  return (current);
}


//
// 'FileIcon::draw()' - Draw an icon.
//

void
FileIcon::draw(int      x,	// I - Upper-lefthand X
               int      y,	// I - Upper-lefthand Y
	       int      w,	// I - Width of bounding box
	       int	h,	// I - Height of bounding box
               Fl_Color ic,	// I - Icon color...
               int      active)	// I - Active or inactive?
{
  Fl_Color	c;		// Current color
  short		*d;		// Pointer to data
  short		*prim;		// Pointer to start of primitive...
  double	scale;		// Scale of icon


  // Don't try to draw a NULL array!
  if (num_data_ == 0)
    return;

  // Setup the transform matrix as needed...
  scale = w < h ? w : h;

  fl_push_matrix();
  fl_translate((float)x + 0.5 * ((float)w - scale),
               (float)y + 0.5 * ((float)h + scale));
  fl_scale(scale, -scale);

  // Loop through the array until we see an unmatched END...
  d    = data_;
  prim = NULL;
  c    = ic;

  if (active)
    fl_color(c);
  else
    fl_color(inactive(c));

  while (*d != END || prim)
    switch (*d)
    {
      case END :
          switch (*prim)
	  {
	    case LINE :
		fl_end_line();
		break;

	    case CLOSEDLINE :
		fl_end_loop();
		break;

	    case POLYGON :
		fl_end_polygon();
		break;

	    case OUTLINEPOLYGON :
		fl_end_polygon();

                if (active)
		{
                  if (prim[1] == 256)
		    fl_color(ic);
		  else
		    fl_color((Fl_Color)prim[1]);
		}
		else
		{
                  if (prim[1] == 256)
		    fl_color(inactive(ic));
		  else
		    fl_color(inactive((Fl_Color)prim[1]));
		}

		fl_begin_loop();

		prim += 2;
		while (*prim == VERTEX)
		{
		  fl_vertex(prim[1] * 0.0001, prim[2] * 0.0001);
		  prim += 3;
		}

        	fl_end_loop();
		fl_color(c);
		break;
	  }

          prim = NULL;
	  d ++;
	  break;

      case COLOR :
          if (d[1] == 256)
	    c = ic;
	  else
	    c = (Fl_Color)d[1];

          if (!active)
	    c = inactive(c);

          fl_color(c);
	  d += 2;
	  break;

      case LINE :
          prim = d;
	  d ++;
	  fl_begin_line();
	  break;

      case CLOSEDLINE :
          prim = d;
	  d ++;
	  fl_begin_loop();
	  break;

      case POLYGON :
          prim = d;
	  d ++;
	  fl_begin_polygon();
	  break;

      case OUTLINEPOLYGON :
          prim = d;
	  d += 2;
	  fl_begin_polygon();
	  break;

      case VERTEX :
          if (prim)
	    fl_vertex(d[1] * 0.0001, d[2] * 0.0001);
	  d += 3;
	  break;
    }

  // If we still have an open primitive, close it...
  if (prim)
    switch (*prim)
    {
      case LINE :
	  fl_end_line();
	  break;

      case CLOSEDLINE :
	  fl_end_loop();
	  break;

      case POLYGON :
	  fl_end_polygon();
	  break;

      case OUTLINEPOLYGON :
	  fl_end_polygon();

          if (active)
	  {
            if (prim[1] == 256)
	      fl_color(ic);
	    else
	      fl_color((Fl_Color)prim[1]);
	  }
	  else
	  {
            if (prim[1] == 256)
	      fl_color(inactive(ic));
	    else
	      fl_color(inactive((Fl_Color)prim[1]));
	  }

	  fl_begin_loop();

	  prim += 2;
	  while (*prim == VERTEX)
	  {
	    fl_vertex(prim[1] * 0.0001, prim[2] * 0.0001);
	    prim += 3;
	  }

          fl_end_loop();
	  fl_color(c);
	  break;
    }

  // Restore the transform matrix
  fl_pop_matrix();
}


//
// 'FileIcon::label()' - Set the widget's label to an icon.
//

void
FileIcon::label(Fl_Widget *w)	// I - Widget to label
{
  Fl::set_labeltype(_FL_ICON_LABEL, labeltype, 0);
  w->label(_FL_ICON_LABEL, (const char*)this);
}


//
// 'FileIcon::labeltype()' - Draw the icon label.
//

void
FileIcon::labeltype(const Fl_Label *o,	// I - Label data
                    int            x,	// I - X position of label
		    int            y,	// I - Y position of label
		    int            w,	// I - Width of label
		    int            h,	// I - Height of label
		    Fl_Align       a)	// I - Label alignment (not used)
{
  FileIcon *icon;			// Pointer to icon data


  icon = (FileIcon *)(o->value);

  icon->draw(x, y, w, h, (Fl_Color)(o->color));
}


//
// End of "$Id: FileIcon.cxx,v 1.2 2001-05-26 10:13:48 jle Exp $".
//
