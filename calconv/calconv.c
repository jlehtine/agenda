/* $Id: calconv.c,v 1.1.1.1 2001-03-19 21:13:58 jle Exp $ */

/*-------------------------------------------------------------------------
 * calconv 0.1 - Agenda VR3 schedule database converter
 *                                                                         
 * Copyright (C) 2001 Johannes Lehtinen
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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <db1/db.h>

/**
 * Converts the specified data buffer from old Schedule database format
 * to new database format.
 *
 * @param old data in old format
 * @param new this is setup with the converted data
 */
static void convert(DBT *old, DBT *new);

/**
 * Prints the given error string and exits.
 *
 * @param msg the error message to display
 */
static void error_exit(char *msg);

int main(int argc, char *argv[]) {
  DB *old_db;
  DB *new_db;
  DBT key;
  DBT old_content;
  int status;
  
  /* Check arguments */
  if (argc != 3) {
    fprintf(stderr, "usage: calconv <old database> <converted database>\n");
    exit(1);
  }

  /* Open the old database for reading */
  if ((old_db = dbopen(argv[1], O_RDONLY, 0664, DB_HASH, NULL)) == NULL)
    error_exit("Could not open database.");
  
  /* Create the new database */
  if ((new_db = dbopen(argv[2], O_RDWR | O_CREAT, 0664, DB_HASH, NULL))
      == NULL)
    error_exit("Could not create database.");

  /* Convert item by item */
  for (status = old_db->seq(old_db, &key, &old_content, R_FIRST);
       status == 0;
       status = old_db->seq(old_db, &key, &old_content, R_NEXT)) {
    DBT new_content;
    
    /* Key is okay as it is but we have to fill in couple of new fields */
    convert(&old_content, &new_content);

    /* Add the converted item to the new database */
    new_db->put(new_db, &key, &new_content, 0);
  }

  /* Close the databases */
  old_db->close(old_db);
  new_db->close(new_db);
  exit(0);
}

static void convert(DBT *old, DBT *new) {
  char *buffer;
  int pos = 2;
  int i;
  
  /* Allocate some space for the new content */
  if ((buffer = malloc(old->size + 3)) == NULL)
    error_exit("Memory allocation failed.");

  /* Make copy of the old content */
  memcpy(buffer, old->data, old->size);
  
  /* Find the position of the new extra field to add */
  for (i = 0; i < 10; i++) {
    while (buffer[pos] != 0x1f) {
      pos++;
      if (pos >= old->size)
        error_exit("Unexpected end of old style record.");
    }
    pos++;
  }

  /* Add the extra fields */
  memmove(buffer+pos+3, buffer+pos, old->size - pos);
  buffer[pos] = '0';
  buffer[pos+1] = 0x1f;
  buffer[pos+2] = 0x1f;

  /* Initialize the new content metadata */
  new->size = old->size + 3;
  new->data = buffer;
}

static void error_exit(char *msg) {
  fprintf(stderr, "calconv: %s\n", msg);
  exit(1);
}
