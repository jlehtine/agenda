// $Id: Figure.cc,v 1.5 2001-05-21 00:46:26 jle Exp $

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
