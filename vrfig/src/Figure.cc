// $Id: Figure.cc,v 1.6 2001-05-23 12:47:50 jle Exp $

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

#include <stdio.h>
#include <stdlib.h>
#include <map.h>
#include "Figure.hpp"
#include "vrfig.hpp"

Figure::~Figure() {
  vector<Element *>::iterator i = elements.begin();
  while (i < elements.end()) {
    delete *(i++);
  }
}

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

void Figure::serialize(ostream &os) {

  // Write the start of XML header
  os << "<?xml version=\"1.0\"?>\n";
  os << "<figure xmlns=\"" << vrf_default_namespace << "\"";
  
  // Find the namespaces used by the figure elements and map them
  // to short namespace identifiers. Write the namespace identifiers
  // to the document element.
  int nscount = 0;
  map<const char *, const char *, char_ptr_less> ns_to_nsid;
  ns_to_nsid[vrf_default_namespace] = 0;
  map<const Element *, const char *> element_to_nsid;
  vector<Element *>::const_iterator i = elements.begin();
  while (i < elements.end()) {
    const char *ns = (*i)->get_namespace();
    const char *nsid;
    if (ns_to_nsid.count(ns)) {
      nsid = ns_to_nsid[ns];
    } else {
      char *id = new char[8];
      snprintf(id, 8, "ns%d", nscount++);
      ns_to_nsid[ns] = id;
      nsid = id;
      os << "\n  xmlns:" << nsid << "=\"" << ns << "\"";
    }
    element_to_nsid[*i] = nsid;
    i++;
  }
  os << ">\n";

  // Serialize all the elements
  os << "  <elements>\n";
  i = elements.begin();
  while (i < elements.end()) {
    const char *nsid = element_to_nsid[*i];
    os << "    <";
    if (nsid)
      os << nsid << ":";
    os << (*i)->get_name() << ">\n";
    (*i)->serialize(os, nsid, 6);
    os << "    </";
    if (nsid)
      os << nsid << ":";
    os << (*i)->get_name() << ">\n";
    i++;
  }
  os << "  </elements>\n";
  
  // Close the document
  os << "</figure>\n";
}
