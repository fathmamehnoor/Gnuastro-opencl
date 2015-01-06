/*********************************************************************
Image Crop - Crop a given size from one or multiple images.
Image Crop is part of GNU Astronomy Utilities (AstrUtils) package.

Copyright (C) 2013-2014 Mohammad Akhlaghi
Tohoku University Astronomical Institute, Sendai, Japan.
http://astr.tohoku.ac.jp/~akhlaghi/

AstrUtils is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

AstrUtils is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with AstrUtils. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/
#ifndef ARGS_H
#define ARGS_H

#include <argp.h>

#include "fixedstringmacros.h"
#include "commonargs.h"










/**************************************************************/
/**************        argp.h definitions       ***************/
/**************************************************************/




/* Definition parameters for the argp: */
const char *argp_program_version=SPACK_STRING"\n"COPYRIGHT
  "\n\nWritten by Mohammad Akhlaghi";
const char *argp_program_bug_address=PACKAGE_BUGREPORT;
static char args_doc[] = "[ASCIIcatalog] ASTRdata ...";





const char doc[] =
  /* Before the list of options: */
  TOPHELPINFO
  SPACK_NAME" will create cutouts, thumbnails, postage stamps or crops of "
  "region(s) from input image(s) using image or celestial coordinates. "
  "If muliple crops are desired, a catalog must be provided. When in WCS "
  "mode, if the cut out covers more than one input image, all overlapping "
  "input images will be stitched in the output.\n"
  MOREHELPINFO
  /* After the list of options: */
  "\v"
  PACKAGE_NAME" home page: "PACKAGE_URL;





/* Available letters for short options:

   e l m n t u v
   A B C E F G H J L M O Q R T U X Y Z

   Number keys used: Nothing!

   Options with keys (second structure element) larger than 500 do not
   have a short version.
 */
static struct argp_option options[] =
  {
    {
      0, 0, 0, 0,
      "Operating modes:",
      -1
    },
    {
      "imgmode",
      'I',
      0,
      0,
      "Use image coordinates (x and y).",
      -1
    },
    {
      "wcsmode",
      'W',
      0,
      0,
      "Use WCS coordinates (Ra and Dec).",
      -1
    },






    {
      0, 0, 0, 0,
      "Input:",
      1
    },






    {
      0, 0, 0, 0,
      "Output:",
      2
    },
    {
      "noblank",
      'b',
      0,
      0,
      "Remove parts of the crop box out of input image.",
      2
    },
    {
      "keepblankcenter",
      'k',
      0,
      0,
      "Keep crop if the central parts are not filled.",
      2
    },
    {
      "checkcenter",
      'c',
      "INT",
      0,
      "Side of box (in pixels) to check.",
      2
    },
    {
      "suffix",
      'p',
      "STR",
      0,
      "Suffix (postfix) of cropped images.",
      2
    },





    {
      0, 0, 0, 0,
      "Crop:",
      3
    },
    {
      "racol",
      'f',
      "INT",
      0,
      "Column of Right Ascension (RA) in catalog.",
      3
    },
    {
      "deccol",
      'g',
      "INT",
      0,
      "Column of Declination (Dec) in catalog.",
      3
    },
    {
      "ra",
      'r',
      "FLT",
      0,
      "Right ascension of one crop box center.",
      3
    },
    {
      "dec",
      'd',
      "FLT",
      0,
      "Declination of one crop box center.",
      3
    },
    {
      "xcol",
      'i',
      "INT",
      0,
      "Column of X (first FITS axis) value in catalog.",
      3
    },
    {
      "ycol",
      'j',
      "INT",
      0,
      "Column of Y (second FITS axis) in catalog.",
      3
    },
    {
      "xc",
      'x',
      "FLT",
      0,
      "First axis position for only one crop.",
      3
    },
    {
      "yc",
      'y',
      "FLT",
      0,
      "Second axis position for only one crop.",
      3,
    },
    {
      "iwidth",
      'a',
      "INT",
      0,
      "Image mode width (in pixels).",
      3
    },
    {
      "wwidth",
      'w',
      "FLT",
      0,
      "WCS mode width (in arc seconds).",
      3
    },
    {
      "section",
      's',
      "STR",
      0,
      "Image section string specifying crop range.",
      3
    },
    {
      "zeroisnotblank",
      'z',
      0,
      0,
      "0.0 in float or double images are not blank.",
      3
    },



    {0}
  };





/* Parse a single option: */
static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
  /* Save the arguments structure: */
  struct imgcropparams *p = state->input;

  /* Set the pointer to the common parameters for all programs
     here: */
  state->child_inputs[0]=&p->cp;

  /* In case the user incorrectly uses the equal sign (for example
     with a short format or with space in the long format, then `arg`
     start with (if the short version was called) or be (if the long
     version was called with a space) the equal sign. So, here we
     check if the first character of arg is the equal sign, then the
     user is warned and the program is stopped: */
  if(arg && arg[0]=='=')
    argp_error(state, "Incorrect use of the equal sign (`=`). For short "
	       "options, `=` should not be used and for long options, "
	       "there should be no space between the option, equal sign "
	       "and value.");

  switch(key)
    {

    /* Operating modes: */
    case 'I':
      if(p->up.imgmodeset)
	argp_error(state, "Only one of Image or WCS modes can be chosen.");
      p->imgmode=1;
      p->wcsmode=0;
      p->up.imgmodeset=p->up.wcsmodeset=1;
      break;
    case 'W':
      if(p->up.wcsmodeset)
	argp_error(state, "Only one of Image or WCS modes can be chosen.");
      p->imgmode=0;
      p->wcsmode=1;
      p->up.imgmodeset=p->up.wcsmodeset=1;
      break;





    /* Output parameters: */
    case 'b':
      p->noblank=1;
      break;
    case 'k':
      p->keepblankcenter=1;
      break;
    case 'c':
      sizetlzero(arg, &p->checkcenter, "checkcenter", key, SPACK, NULL, 0);
      p->up.checkcenterset=1;
      break;
    case 'p':
      errno=0;
      p->suffix=malloc(strlen(arg)+1);
      if(p->suffix==NULL) error(EXIT_FAILURE, 0, NULL);
      strcpy(p->suffix, arg);
      p->up.suffixset=1;
      break;





    /* Crop: */
    case 'f':
      sizetelzero(arg, &p->racol, "racol", key, SPACK, NULL, 0);
      p->up.racolset=1;
      break;
    case 'g':
      sizetelzero(arg, &p->deccol, "deccol", key, SPACK, NULL, 0);
      p->up.deccolset=1;
      break;
    case 'r':
      anydouble(arg, &p->ra, "ra", key, SPACK, NULL, 0);
      p->up.raset=1;
      break;
    case 'd':
      anydouble(arg, &p->dec, "dec", key, SPACK, NULL, 0);
      p->up.decset=1;
      break;
    case 'i':
      sizetelzero(arg, &p->xcol, "xcol", key, SPACK, NULL, 0);
      p->up.xcolset=1;
      break;
    case 'j':
      sizetelzero(arg, &p->ycol, "ycol", key, SPACK, NULL, 0);
      p->up.ycolset=1;
      break;
    case 'x':
      anydouble(arg, &p->xc, "xc", key, SPACK, NULL, 0);
      p->up.xcset=1;		/* Using FITS standard, not C. */
      break;
    case 'y':
      anydouble(arg, &p->yc, "yc", key, SPACK, NULL, 0);
      p->up.ycset=1;		/* Using FITS standard, not C. */
      break;
    case 'a':
      sizetlzero(arg, &p->iwidth, "iwidth", key, SPACK, NULL, 0);
      p->up.iwidthset=1;
      break;
    case 'w':
      doublel0(arg, &p->wwidth, "wwidth", key, SPACK, NULL, 0);
      p->up.wwidthset=1;
      break;
    case 's':
      p->section=arg;
      p->up.sectionset=1;
      break;
    case 'z':
      p->zeroisnotblank=1;
      break;






    /* Read the non-option arguments: */
    case ARGP_KEY_ARG:

      /* See what type of input value it is and put it in. */
      if( nameisfits(arg) )
	{
	  add_to_stll(&p->up.stll, arg);
	  ++p->numimg;
	}
      else
	{
	  if(p->up.catname)
	    argp_error(state, "Only one catalog file can be given.");
	  else
	    {
	      p->up.catname=arg;
	      p->up.catset=1;
	    }
	}
      break;





    /* The command line options and arguments are finished. */
    case ARGP_KEY_END:
      if(p->cp.setdirconf==0 && p->cp.setusrconf==0
	 && p->cp.printparams==0)
	{
	  if(state->arg_num==0)
	    argp_error(state, "No argument given!");
	  if(p->up.catname==NULL && !(p->up.xcset    || p->up.ycset
				      || p->up.raset || p->up.decset
				      || p->up.sectionset))
	    argp_error(state, "No catalog provided!");
	  if(p->up.stll==NULL)
	    argp_error(state, "No FITS image(s) provided!");
	}
      break;





    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}





/* Specify the children parsers: */
struct argp_child children[]=
  {
    {&commonargp, 0, NULL, 0},
    {0, 0, 0, 0}
  };





/* Basic structure defining the whole argument reading process. */
static struct argp thisargp = {options, parse_opt, args_doc,
			       doc, children, NULL, NULL};

#endif