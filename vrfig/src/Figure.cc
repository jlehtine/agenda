// $Id: Figure.cc,v 1.2 2001-05-17 19:35:27 jle Exp $

#include <stdio.h>
#include <stdlib.h>
#include <map.h>
#include "Figure.hpp"
#include "vrfig.hpp"

void Figure::serialize(ostream &os) {

  // Write the start of XML header
  os << "<?xml version=\"1.0\"?>\n";
  os << "<figure xmlns=\"" << vrf_default_namespace << "\"";
  
  // Find the namespaces used by the figure elements and map them
  // to short namespace identifiers. Write the namespace identifiers
  // to the document element.
  int nscount = 0;
  map<string *, string *> ns_to_nsid;
  ns_to_nsid[const_cast<string *>(&vrf_default_namespace)] = 0;
  map<Element *, string *> element_to_nsid;
  vector<Element *>::const_iterator i = elements.begin();
  while (i < elements.end()) {
    const string *ns = (*i)->get_namespace();
    const string *nsid;
    if (ns_to_nsid.count(const_cast<string *>(ns))) {
      nsid = ns_to_nsid[const_cast<string *>(ns)];
    } else {
      string *id = new string("ns", 5);
      char num[5];
      snprintf(num, 5, "%d", nscount++);
      id->append(num);
      ns_to_nsid[(string *)ns] = id;
      nsid = id;
      os << "\n  xmlns:" << nsid << "=\"" << ns << "\"";
    }
    element_to_nsid[*i] = const_cast<string *>(nsid);
    i++;
  }
  os << ">\n";

  // Serialize all the elements
  os << "  <elements>\n";
  i = elements.begin();
  while (i < elements.end()) {
    const string *nsid = element_to_nsid[*i];
    cout << "    <" << nsid << ":" << (*i)->get_name() << ">\n";
    (*i)->serialize(os, nsid, 6);
    cout << "    </" << nsid << ":" << (*i)->get_name() << ">\n";
  }
  os << "  </elements>\n";
  
  // Close the document
  os << "</figure>\n";
}
