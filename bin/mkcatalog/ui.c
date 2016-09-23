/*********************************************************************
MakeCatalog - Make a catalog from an input and labeled image.
MakeCatalog is part of GNU Astronomy Utilities (Gnuastro) package.

Original author:
     Mohammad Akhlaghi <akhlaghi@gnu.org>
Contributing author(s):
Copyright (C) 2015, Free Software Foundation, Inc.

Gnuastro is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

Gnuastro is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gnuastro. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/
#include <config.h>

#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <string.h>
#include <fitsio.h>

#include <nproc.h>               /* From Gnulib.                   */

#include <gnuastro/fits.h>
#include <gnuastro/timing.h>     /* Includes time.h and sys/time.h */
#include <gnuastro/checkset.h>
#include <gnuastro/txtarray.h>
#include <gnuastro/statistics.h>
#include <gnuastro/commonargs.h>
#include <gnuastro/configfiles.h>

#include "main.h"

#include "ui.h"                  /* Needs main.h                   */
#include "args.h"                /* Needs main.h, includes argp.h. */


/* Set the file names of the places where the default parameters are
   put. */
#define CONFIG_FILE SPACK CONF_POSTFIX
#define SYSCONFIG_FILE SYSCONFIG_DIR "/" CONFIG_FILE
#define USERCONFIG_FILEEND USERCONFIG_DIR CONFIG_FILE
#define CURDIRCONFIG_FILE CURDIRCONFIG_DIR CONFIG_FILE










/**************************************************************/
/**************       Options and parameters    ***************/
/**************************************************************/
void
readconfig(char *filename, struct mkcatalogparams *p)
{
  int yes;
  FILE *fp;
  size_t lineno=0, len=200;
  char *line, *name, *value;
  struct uiparams *up=&p->up;
  struct gal_commonparams *cp=&p->cp;
  char key='a';        /* Not used, just a place holder. */

  /* When the file doesn't exist or can't be opened, it is ignored. It
     might be intentional, so there is no error. If a parameter is
     missing, it will be reported after all defaults are read. */
  fp=fopen(filename, "r");
  if (fp==NULL) return;


  /* Allocate some space for `line` with `len` elements so it can
     easily be freed later on. The value of `len` is arbitarary at
     this point, during the run, getline will change it along with the
     pointer to line. */
  errno=0;
  line=malloc(len*sizeof *line);
  if(line==NULL)
    error(EXIT_FAILURE, errno, "ui.c: %lu bytes in readdefaults",
          len * sizeof *line);

  /* Read the tokens in the file:  */
  while(getline(&line, &len, fp) != -1)
    {
      /* Prepare the "name" and "value" strings, also set lineno. */
      GAL_CONFIGFILES_START_READING_LINE;




      /* Inputs: */
      if(strcmp(name, "hdu")==0)
        gal_checkset_allocate_copy_set(value, &cp->hdu, &cp->hduset);
      else if (strcmp(name, "mask")==0)
        gal_checkset_allocate_copy_set(value, &up->maskname,
                                       &up->masknameset);
      else if (strcmp(name, "mhdu")==0)
        gal_checkset_allocate_copy_set(value, &up->mhdu, &up->mhduset);
      else if (strcmp(name, "objlabs")==0)
        gal_checkset_allocate_copy_set(value, &up->objlabsname,
                                       &up->objlabsnameset);
      else if (strcmp(name, "objhdu")==0)
        gal_checkset_allocate_copy_set(value, &up->objhdu,
                                       &up->objhduset);
      else if (strcmp(name, "clumplabs")==0)
        gal_checkset_allocate_copy_set(value, &up->clumplabsname,
                                       &up->clumplabsnameset);
      else if (strcmp(name, "clumphdu")==0)
        gal_checkset_allocate_copy_set(value, &up->clumphdu,
                                       &up->clumphduset);
      else if (strcmp(name, "skyfilename")==0)
        gal_checkset_allocate_copy_set(value, &up->skyname,
                                       &up->skynameset);
      else if (strcmp(name, "skyhdu")==0)
        gal_checkset_allocate_copy_set(value, &up->skyhdu,
                                       &up->skyhduset);
      else if (strcmp(name, "stdfilename")==0)
        gal_checkset_allocate_copy_set(value, &up->stdname,
                                       &up->stdnameset);
      else if (strcmp(name, "stdhdu")==0)
        gal_checkset_allocate_copy_set(value, &up->stdhdu,
                                       &up->stdhduset);
      else if (strcmp(name, "zeropoint")==0)
        {
          if(up->zeropointset) continue;
          gal_checkset_any_float(value, &p->zeropoint, name, key, SPACK,
                                 filename, lineno);
          up->zeropointset=1;
        }
      else if (strcmp(name, "skysubtracted")==0)
        {
          if(up->skysubtractedset) continue;
          gal_checkset_int_zero_or_one(value, &p->skysubtracted, name,
                                       key, SPACK, filename, lineno);
          up->skysubtractedset=1;
        }
      else if(strcmp(name, "threshold")==0)
        {
          if(up->thresholdset) continue;
          gal_checkset_any_double(value, &p->threshold, name, key,
                                  SPACK, filename, lineno);
          up->thresholdset=1;
        }


      /* Outputs */
      else if(strcmp(name, "output")==0)
        gal_checkset_allocate_copy_set(value, &cp->output,
                                       &cp->outputset);
      else if(strcmp(name, "nsigmag")==0)
        {
          if(up->nsigmagset) continue;
          gal_checkset_any_double(value, &p->nsigmag, name, key, SPACK,
                                  filename, lineno);
          up->nsigmagset=1;
        }
      else if(strcmp(name, "intwidth")==0)
        {
          if(up->intwidthset) continue;
          gal_checkset_int_l_zero(value, &p->intwidth, name, key, SPACK,
                                  filename, lineno);
          up->intwidthset=1;
        }
      else if(strcmp(name, "floatwidth")==0)
        {
          if(up->floatwidthset) continue;
          gal_checkset_int_l_zero(value, &p->floatwidth, name, key,
                                  SPACK, filename, lineno);
          up->floatwidthset=1;
        }
      else if(strcmp(name, "accuwidth")==0)
        {
          if(up->accuwidthset) continue;
          gal_checkset_int_l_zero(value, &p->accuwidth, name, key,
                                  SPACK, filename, lineno);
          up->accuwidthset=1;
        }
      else if(strcmp(name, "floatprecision")==0)
        {
          if(up->floatprecisionset) continue;
          gal_checkset_int_l_zero(value, &p->floatprecision, name,
                                  key, SPACK, filename, lineno);
          up->floatprecisionset=1;
        }
      else if(strcmp(name, "accuprecision")==0)
        {
          if(up->accuprecisionset) continue;
          gal_checkset_int_l_zero(value, &p->accuprecision, name,
                                  key, SPACK, filename, lineno);
          up->accuprecisionset=1;
        }


      /* Catalog columns */
      else if(strcmp(name, "id")==0)
        {
          if(up->idset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATID);
          up->idset=1;
        }
      else if(strcmp(name, "hostobjid")==0)
        {
          if(up->hostobjidset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATHOSTOBJID);
          up->hostobjidset=1;
        }
      else if(strcmp(name, "idinhostobj")==0)
        {
          if(up->idinhostobjset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATIDINHOSTOBJ);
          up->idinhostobjset=1;
        }
      else if(strcmp(name, "numclumps")==0)
        {
          if(up->numclumpsset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATNUMCLUMPS);
          up->numclumpsset=1;
        }
      else if(strcmp(name, "area")==0)
        {
          if(up->areaset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATAREA);
          up->areaset=1;
        }
      else if(strcmp(name, "clumpsarea")==0)
        {
          if(up->clumpsareaset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSAREA);
          up->clumpsareaset=1;
        }
      else if(strcmp(name, "x")==0)
        {
          if(up->xset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATX);
          up->xset=1;
        }
      else if(strcmp(name, "y")==0)
        {
          if(up->yset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATY);
          up->yset=1;
        }
      else if(strcmp(name, "geox")==0)
        {
          if(up->geoxset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATGEOX);
          up->geoxset=1;
        }
      else if(strcmp(name, "geoy")==0)
        {
          if(up->geoyset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATGEOY);
          up->geoyset=1;
        }
      else if(strcmp(name, "clumpsx")==0)
        {
          if(up->clumpsxset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSX);
          up->clumpsxset=1;
        }
      else if(strcmp(name, "clumpsy")==0)
        {
          if(up->clumpsyset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSY);
          up->clumpsyset=1;
        }
      else if(strcmp(name, "clumpsgeox")==0)
        {
          if(up->clumpsgeoxset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSGEOX);
          up->clumpsgeoxset=1;
        }
      else if(strcmp(name, "clumpsgeoy")==0)
        {
          if(up->clumpsgeoyset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSGEOY);
          up->clumpsgeoyset=1;
        }
      else if(strcmp(name, "ra")==0)
        {
          if(up->raset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATRA);
          up->raset=1;
        }
      else if(strcmp(name, "dec")==0)
        {
          if(up->decset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATDEC);
          up->decset=1;
        }
      else if(strcmp(name, "geora")==0)
        {
          if(up->georaset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATGEORA);
          up->georaset=1;
        }
      else if(strcmp(name, "geodec")==0)
        {
          if(up->geodecset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATGEODEC);
          up->geodecset=1;
        }
      else if(strcmp(name, "clumpsra")==0)
        {
          if(up->clumpsraset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSRA);
          up->clumpsraset=1;
        }
      else if(strcmp(name, "clumpsdec")==0)
        {
          if(up->clumpsdecset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSDEC);
          up->clumpsdecset=1;
        }
      else if(strcmp(name, "clumpsgeora")==0)
        {
          if(up->clumpsgeoraset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSGEORA);
          up->clumpsgeoraset=1;
        }
      else if(strcmp(name, "clumpsgeodec")==0)
        {
          if(up->clumpsgeodecset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSGEODEC);
          up->clumpsgeodecset=1;
        }
      else if(strcmp(name, "brightness")==0)
        {
          if(up->brightnessset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATBRIGHTNESS);
          up->brightnessset=1;
        }
      else if(strcmp(name, "clumpsbrightness")==0)
        {
          if(up->clumpsbrightnessset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSBRIGHTNESS);
          p->up.clumpsbrightnessset=1;
        }
      else if(strcmp(name, "noriverbrightness")==0)
        {
          if(up->noriverbrightnessset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATNORIVERBRIGHTNESS);
          p->up.noriverbrightnessset=1;
        }
      else if(strcmp(name, "magnitude")==0)
        {
          if(up->magnitudeset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATMAGNITUDE);
          up->magnitudeset=1;
        }
      else if(strcmp(name, "magnitudeerr")==0)
        {
          if(up->magnitudeerrset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATMAGNITUDEERR);
          up->magnitudeerrset=1;
        }
      else if(strcmp(name, "clumpsmagnitude")==0)
        {
          if(up->clumpsmagnitudeset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATCLUMPSMAGNITUDE);
          up->clumpsmagnitudeset=1;
        }
      else if(strcmp(name, "riverave")==0)
        {
          if(up->riveraveset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATRIVERAVE);
          up->riveraveset=1;
        }
      else if(strcmp(name, "rivernum")==0)
        {
          if(up->rivernumset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATRIVERNUM);
          up->rivernumset=1;
        }
      else if(strcmp(name, "sn")==0)
        {
          if(up->snset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATSN);
          up->snset=1;
        }
      else if(strcmp(name, "sky")==0)
        {
          if(up->skyset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATSKY);
          up->skyset=1;
        }
      else if(strcmp(name, "std")==0)
        {
          if(up->stdset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATSTD);
          up->stdset=1;
        }
      else if(strcmp(name, "semimajor")==0)
        {
          if(up->semimajorset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATSEMIMAJOR);
          up->semimajorset=1;
        }
      else if(strcmp(name, "semiminor")==0)
        {
          if(up->semiminorset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATSEMIMINOR);
          up->semiminorset=1;
        }
      else if(strcmp(name, "positionangle")==0)
        {
          if(up->positionangleset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATPOSITIONANGLE);
          up->positionangleset=1;
        }
      else if(strcmp(name, "geosemimajor")==0)
        {
          if(up->geosemimajorset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATGEOSEMIMAJOR);
          up->geosemimajorset=1;
        }
      else if(strcmp(name, "geosemiminor")==0)
        {
          if(up->geosemiminorset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATGEOSEMIMINOR);
          up->geosemiminorset=1;
        }
      else if(strcmp(name, "geopositionangle")==0)
        {
          if(up->geopositionangleset) continue;
          gal_checkset_int_zero_or_one(value, &yes, name, key, SPACK,
                                       filename, lineno);
          if(!yes) continue;
          gal_linkedlist_add_to_sll(&p->allcolsll, CATGEOPOSITIONANGLE);
          up->geopositionangleset=1;
        }



      /* Operating modes */
      /* Read options common to all programs */
      GAL_CONFIGFILES_READ_COMMONOPTIONS_FROM_CONF


      else
        error_at_line(EXIT_FAILURE, 0, filename, lineno,
                      "`%s` not recognized.\n", name);
    }

  free(line);
  fclose(fp);
}





void
printvalues(FILE *fp, struct mkcatalogparams *p)
{
  int i;
  struct uiparams *up=&p->up;
  struct gal_commonparams *cp=&p->cp;


  /* Print all the options that are set. Separate each group with a
     commented line explaining the options in that group. */
  fprintf(fp, "\n# Input image:\n");
  if(cp->hduset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("hdu", cp->hdu);
  if(up->masknameset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("mask", up->maskname);
  if(up->mhduset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("mhdu", up->mhdu);
  if(up->objlabsnameset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("objlabs",
                                               up->objlabsname);
  if(up->objhduset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("objhdu", up->objhdu);
  if(up->clumplabsnameset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("clumplabs",
                                               up->clumplabsname);
  if(up->clumphduset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("clumphdu",
                                               up->clumphdu);
  if(up->skynameset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("skyfilename",
                                               up->skyname);
  if(up->skyhduset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("skyhdu", up->skyhdu);
  if(up->stdnameset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("stdfilename",
                                               up->stdname);
  if(up->stdhduset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("stdhdu", up->stdhdu);
  if(up->zeropointset)
    fprintf(fp, CONF_SHOWFMT"%.3f\n", "zeropoint", p->zeropoint);
  if(up->skysubtractedset)
    fprintf(fp, CONF_SHOWFMT"%d\n", "skysubtracted", p->skysubtracted);
  if(up->thresholdset)
    fprintf(fp, CONF_SHOWFMT"%.3f\n", "threshold", p->threshold);

  /* Output: */
  fprintf(fp, "\n# Output:\n");
  if(cp->outputset)
    GAL_CHECKSET_PRINT_STRING_MAYBE_WITH_SPACE("output", cp->output);
  if(up->nsigmagset)
    fprintf(fp, CONF_SHOWFMT"%g\n", "nsigmag", p->nsigmag);
  if(up->intwidthset)
    fprintf(fp, CONF_SHOWFMT"%d\n", "intwidth", p->intwidth);
  if(up->floatwidthset)
    fprintf(fp, CONF_SHOWFMT"%d\n", "floatwidth", p->floatwidth);
  if(up->accuwidthset)
    fprintf(fp, CONF_SHOWFMT"%d\n", "accuwidth", p->accuwidth);
  if(up->floatprecisionset)
    fprintf(fp, CONF_SHOWFMT"%d\n", "floatprecision", p->floatprecision);
  if(up->accuprecisionset)
    fprintf(fp, CONF_SHOWFMT"%d\n", "accuprecision", p->accuprecision);

  /* Catalog columns, since order is important. Notice that they have
     to be printed in opposite order (because of the way they are read
     through a simple linked list). */
  fprintf(fp, "\n# Catalog columns:\n");
  for(i=p->allncols-1;i>=0;--i)
    switch(p->allcols[i])
      {
      case CATID:
        fprintf(fp, CONF_SHOWFMT"%d\n", "id", 1);
        break;
      case CATHOSTOBJID:
        fprintf(fp, CONF_SHOWFMT"%d\n", "hostobjid", 1);
        break;
      case CATIDINHOSTOBJ:
        fprintf(fp, CONF_SHOWFMT"%d\n", "idinhostobj", 1);
        break;
      case CATNUMCLUMPS:
        fprintf(fp, CONF_SHOWFMT"%d\n", "numclumps", 1);
        break;
      case CATAREA:
        fprintf(fp, CONF_SHOWFMT"%d\n", "area", 1);
        break;
      case CATCLUMPSAREA:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsarea", 1);
        break;
      case CATX:
        fprintf(fp, CONF_SHOWFMT"%d\n", "x", 1);
        break;
      case CATY:
        fprintf(fp, CONF_SHOWFMT"%d\n", "y", 1);
        break;
      case CATGEOX:
        fprintf(fp, CONF_SHOWFMT"%d\n", "x", 1);
        break;
      case CATGEOY:
        fprintf(fp, CONF_SHOWFMT"%d\n", "y", 1);
        break;
      case CATCLUMPSX:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsx", 1);
        break;
      case CATCLUMPSY:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsy", 1);
        break;
      case CATCLUMPSGEOX:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsgeox", 1);
        break;
      case CATCLUMPSGEOY:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsgeoy", 1);
        break;
      case CATRA:
        fprintf(fp, CONF_SHOWFMT"%d\n", "ra", 1);
        break;
      case CATDEC:
        fprintf(fp, CONF_SHOWFMT"%d\n", "dec", 1);
        break;
      case CATGEORA:
        fprintf(fp, CONF_SHOWFMT"%d\n", "geora", 1);
        break;
      case CATGEODEC:
        fprintf(fp, CONF_SHOWFMT"%d\n", "geodec", 1);
        break;
      case CATCLUMPSRA:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsra", 1);
        break;
      case CATCLUMPSDEC:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsdec", 1);
        break;
      case CATCLUMPSGEORA:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsgeora", 1);
        break;
      case CATCLUMPSGEODEC:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsgeodec", 1);
        break;
      case CATBRIGHTNESS:
        fprintf(fp, CONF_SHOWFMT"%d\n", "brightness", 1);
        break;
      case CATCLUMPSBRIGHTNESS:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsbrightness", 1);
        break;
      case CATNORIVERBRIGHTNESS:
        fprintf(fp, CONF_SHOWFMT"%d\n", "noriverbrightness", 1);
        break;
      case CATMAGNITUDE:
        fprintf(fp, CONF_SHOWFMT"%d\n", "magnitude", 1);
        break;
      case CATMAGNITUDEERR:
        fprintf(fp, CONF_SHOWFMT"%d\n", "magnitudeerr", 1);
        break;
      case CATCLUMPSMAGNITUDE:
        fprintf(fp, CONF_SHOWFMT"%d\n", "clumpsmagnitude", 1);
        break;
      case CATRIVERAVE:
        fprintf(fp, CONF_SHOWFMT"%d\n", "riverave", 1);
        break;
      case CATRIVERNUM:
        fprintf(fp, CONF_SHOWFMT"%d\n", "rivernum", 1);
        break;
      case CATSN:
        fprintf(fp, CONF_SHOWFMT"%d\n", "sn", 1);
        break;
      case CATSKY:
        fprintf(fp, CONF_SHOWFMT"%d\n", "sky", 1);
        break;
      case CATSTD:
        fprintf(fp, CONF_SHOWFMT"%d\n", "std", 1);
        break;
      case CATSEMIMAJOR:
        fprintf(fp, CONF_SHOWFMT"%d\n", "semimajor", 1);
        break;
      case CATSEMIMINOR:
        fprintf(fp, CONF_SHOWFMT"%d\n", "semiminor", 1);
        break;
      case CATPOSITIONANGLE:
        fprintf(fp, CONF_SHOWFMT"%d\n", "positionangle", 1);
        break;
      case CATGEOSEMIMAJOR:
        fprintf(fp, CONF_SHOWFMT"%d\n", "geosemimajor", 1);
        break;
      case CATGEOSEMIMINOR:
        fprintf(fp, CONF_SHOWFMT"%d\n", "geosemiminor", 1);
        break;
      case CATGEOPOSITIONANGLE:
        fprintf(fp, CONF_SHOWFMT"%d\n", "geopositionangle", 1);
        break;
      default:
        error(EXIT_FAILURE, 0, "a bug! Please contact us at %s so we can "
              "fix the problem. For some reason p->allcols[%d] is given "
              "the value %lu which is not recognized in printvalues "
              "(ui.c)", PACKAGE_BUGREPORT, i, p->allcols[i]);
      }

  /* For the operating mode, first put the macro to print the common
     options, then the (possible options particular to this
     program). */
  fprintf(fp, "\n# Operating mode:\n");
  GAL_CONFIGFILES_PRINT_COMMONOPTIONS;
}






/* Note that numthreads will be used automatically based on the
   configure time. */
void
checkifset(struct mkcatalogparams *p)
{
  struct uiparams *up=&p->up;
  struct gal_commonparams *cp=&p->cp;

  int intro=0;
  if(cp->hduset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("hdu");
  if(up->objhduset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("objhdu");
  if(up->clumphduset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("clumphdu");
  if(up->skyhduset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("skyhdu");
  if(up->stdhduset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("stdhdu");
  if(up->zeropointset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("zeropoint");
  if(up->skysubtractedset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("skysubtracted");

  /* Output: */
  if(up->nsigmagset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("nsigmag");
  if(up->intwidthset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("intwidth");
  if(up->floatwidthset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("floatwidth");
  if(up->accuwidthset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("accuwidth");
  if(up->floatprecisionset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("floatprecision");
  if(up->accuprecisionset==0)
    GAL_CONFIGFILES_REPORT_NOTSET("accuprecision");

  GAL_CONFIGFILES_END_OF_NOTSET_REPORT;
}




















/**************************************************************/
/***************       Sanity Check         *******************/
/**************************************************************/
void
sanitycheck(struct mkcatalogparams *p)
{
  struct uiparams *up=&p->up;
  struct gal_fits_key keys[1];
  struct gal_commonparams *cp=&p->cp;

  /* Make sure the input file exists. */
  gal_checkset_check_file(up->inputname);

  /* Set the names of the files. */
  gal_fits_file_or_ext_name(up->inputname, p->cp.hdu, up->masknameset,
                            &up->maskname, up->mhdu, up->mhduset,
                            "mask");
  gal_fits_file_or_ext_name(up->inputname, p->cp.hdu,
                            up->objlabsnameset, &up->objlabsname,
                            up->objhdu, up->objhduset,
                            "object labels");
  gal_fits_file_or_ext_name(up->inputname, p->cp.hdu, up->skynameset,
                            &up->skyname, up->skyhdu, up->skyhduset,
                            "sky value image");
  gal_fits_file_or_ext_name(up->inputname, p->cp.hdu, up->stdnameset,
                            &up->stdname, up->stdhdu, up->stdhduset,
                            "sky standard deviation");

  /* The WCLUMPS (with-clumps) keyword in the object HDU says that there is
     also a clumps image accompaning the object image. If it exists and its
     value is `yes' (not case sensitive), then set the image name to the
     proper string. Otherwise, set the image name to NULL, so future
     functions can check. */
  keys[0].str[0]='\0';
  keys[0].datatype=TSTRING;
  keys[0].keyname="WCLUMPS";
  gal_fits_read_keywords(up->objlabsname, up->objhdu, keys, 1);
  if(strcasecmp(keys[0].str, "yes"))
    up->clumplabsname=NULL;
  else
    gal_fits_file_or_ext_name(up->inputname, p->cp.hdu,
                              up->clumplabsnameset, &up->clumplabsname,
                              up->clumphdu, up->clumphduset,
                              "clump labels");

  /* When the RA and Dec are needed, make sure that the X and Y
     columns and the RA and Dec columns in the information array are
     immediately after each other and in this order. This is not set
     by the user, but by the programmer. Nevertheless, it is a very
     important issue to check. Since the enum values are constants,
     the compiler will know immediately and stop.

     NOTE: the information array is separate from the output array
  */
  if(up->raset || up->decset)
    {
      if( OFlxWhtX!=OFlxWhtY-1 || OFlxWhtRA!=OFlxWhtDec-1 )
        error(EXIT_FAILURE, 0, "a bug! Please contact us at %s so we can "
              "fix the problem. X(%d) and Y(%d), or Ra(%d) and Dec(%d) "
              "columns in the information array are not immediately "
              "after each other", PACKAGE_BUGREPORT, OFlxWhtX, OFlxWhtY,
              OFlxWhtRA, OFlxWhtDec);
    }

  /* Set the output names, based on if the output is set or not. */
  gal_checkset_automatic_output(cp->outputset ? cp->output : up->inputname,
                                up->clumplabsname ? "_o.txt" : ".txt",
                                cp->outputset ? 0 : p->cp.removedirinfo,
                                p->cp.dontdelete, &p->ocatname);
  if(up->clumplabsname)
    gal_checkset_automatic_output(cp->outputset ? cp->output : up->inputname,
                                  "_c.txt",
                                  cp->outputset ? 0 : p->cp.removedirinfo,
                                  p->cp.dontdelete, &p->ccatname);
}



















/**************************************************************/
/***************       Preparations         *******************/
/**************************************************************/
void
checksetlong(struct mkcatalogparams *p, char *filename, char *hdu,
             long **array)
{
  size_t s0, s1;
  int bitpix, anyblank;

  /* Read the file: */
  gal_fits_file_to_long(filename, hdu, array, &bitpix,
                        &anyblank, &s0, &s1);

  /* Make sure it has an integer type. */
  if(bitpix==FLOAT_IMG || bitpix==DOUBLE_IMG)
    error(EXIT_FAILURE, 0, "the labels image can be any integer type "
          "(BITPIX). However, %s (hdu: %s) is a %s precision floating "
          "point image", filename, hdu,
          bitpix==FLOAT_IMG ? "single" : "double");

  /* Make sure it is the same size as the input image. */
  if(s0!=p->s0 || s1!=p->s1)
    error(EXIT_FAILURE, 0, "%s (hdu: %s) is %lu x %lu pixels while the "
          "%s (hdu: %s) is %lu x %lu. The images should have the same "
          "size", filename, hdu, s1, s0, p->up.inputname,
          p->cp.hdu, p->s1, p->s0);
}





void
checksetfloat(struct mkcatalogparams *p, char *filename, char *hdu,
              float **array)
{
  size_t s0, s1;
  int bitpix, anyblank;

  /* Read the array: */
  gal_fits_file_to_float(filename, NULL, hdu, NULL, array, &bitpix,
                              &anyblank, &s0, &s1);

  /* Make sure it has no blank pixels.
  if(anyblank)
    error(EXIT_FAILURE, 0, "the Sky and Sky standard deviation images "
          "should not have any blank values. %s (hdu: %s) has blank "
          "pixels", filename, hdu);
  */

  /* Make sure it has the same size as the image. */
  if(s0!=p->s0 || s1!=p->s1)
    error(EXIT_FAILURE, 0, "%s (hdu: %s) is %lu x %lu pixels while the "
          "%s (hdu: %s) is %lu x %lu. The images should have the same "
          "size", filename, hdu, s1, s0, p->up.inputname,
          p->cp.hdu, p->s1, p->s0);
}





/* Read the necessary keywords and if they aren't present do the
   appropriate action. */
void
readkeywords(struct mkcatalogparams *p)
{
  long numobjects;
  size_t size=p->s0*p->s1;
  struct uiparams *up=&p->up;
  struct gal_fits_key keys[2];

  /* Read keywords from the standard deviation image:  */
  keys[0].keyname="MINSTD";   keys[0].datatype=TFLOAT;
  keys[1].keyname="MEDSTD";   keys[1].datatype=TFLOAT;
  gal_fits_read_keywords(up->stdname, up->stdhdu, keys, 2);

  /* The minimum standard deviation value. */
  if(keys[0].status)
    gal_statistics_float_min(p->std, size, &p->minstd);
  else
    p->minstd=keys[0].f;
  p->cpscorr = p->minstd>1 ? 1.0f : p->minstd;

  /* Median standard deviation value (only necessary in catalog
     comments). */
  if(keys[1].status)
    {
      p->medstd=gal_statistics_median(p->std, size);
      fprintf(stderr, "Warning: Could not find the MEDSTD keyword in %s "
              "(hdu: %s). The median standard deviation is thus found on "
              "the (interpolated) standard deviation image. NoiseChisel "
              "finds the median before interpolation, so the reported value "
              "in the final catalog will not be accurate (will depend on "
              "how many meshs were blank and their spatial position "
              "relative to the non-blank ones.", up->stdname, up->stdhdu);
    }
  else
    p->medstd=keys[1].f;

  /* Read the keywords from the objects image:  */
  keys[0].keyname="DETSN";     keys[0].datatype=TDOUBLE;
  keys[1].keyname="NOBJS";     keys[1].datatype=TLONG;
  gal_fits_read_keywords(up->objlabsname, up->objhdu, keys, 2);

  /* If DETSN is not given, there is no way we can calculate it here, so we
     will just set it to NaN to check and not report in the end. */
  p->detsn = keys[0].status ? NAN : keys[0].d;

  /* Read the total number of objects. */
  if(keys[1].status)
    {
      gal_statistics_long_non_blank_max(p->objects, size, &numobjects,
                                        GAL_FITS_LONG_BLANK);
      p->numobjects=numobjects;
    }
  else
    p->numobjects=keys[1].l;

  /* Read the clumps information if it is necessary.

     Note that unlike finding the number of objects, finding the number of
     clumps is not an easy process, since the clumps of each object start
     with a label of one. So if the number of clumps is not given, we have
     to abort. For now, is up to the program that built the clumps to give
     the total number, later we can take a procedure to find them (for
     example first only taking the positive labels (that are clumps) and
     making a binary image, then running the connected component algorithm
     to find the number of clumps in the image.*/
  if(up->clumplabsname)
    {
      keys[0].keyname="CLUMPSN";
      keys[1].keyname="NCLUMPS";
      gal_fits_read_keywords(up->clumplabsname, up->clumphdu, keys, 2);
      p->clumpsn = keys[0].status ? NAN : keys[0].d;
      if(keys[1].status)
        error(EXIT_FAILURE, 0, "couldn't find NCLUMPS in the header of "
              "%s (hdu: %s).", up->clumplabsname, up->clumphdu);
      else
        p->numclumps=keys[1].l;
    }

}






void
preparearrays(struct mkcatalogparams *p)
{
  size_t i;
  int bitpix, anyblank;


  /* Prepare the columns and allocate the p->objcols and p->clumpcols
     arrays to keep the macros of what output they should keep. */
  gal_linkedlist_sll_to_array(p->allcolsll, &p->allcols, &p->allncols, 0);
  if(p->allncols==0)
    error(EXIT_FAILURE, 0, "no columns specified for output");
  errno=0; p->objcols=malloc(p->allncols*sizeof *p->objcols);
  if(p->objcols==NULL)
    error(EXIT_FAILURE, errno, "%lu bytes for p->objcols in "
          "preprarearrays (ui.c)", p->allncols*sizeof *p->objcols);
  errno=0; p->clumpcols=malloc(p->allncols*sizeof *p->clumpcols);
  if(p->clumpcols==NULL)
    error(EXIT_FAILURE, errno, "%lu bytes for p->clumpcols in "
          "preprarearrays (ui.c)", p->allncols*sizeof *p->clumpcols);


  /* Put all the input columns into the object or clumps columns and
     count how many columns each should have. */
  p->objncols=p->clumpncols=0;
  for(i=0;i<p->allncols;++i)
    {
      switch(p->allcols[i])
        {
        case CATID:
          p->objcols[p->objncols++]=p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATHOSTOBJID:
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATIDINHOSTOBJ:
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATNUMCLUMPS:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATAREA:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATCLUMPSAREA:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATX:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATY:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATGEOX:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATGEOY:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATCLUMPSX:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATCLUMPSY:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATCLUMPSGEOX:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATCLUMPSGEOY:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATRA:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATDEC:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATGEORA:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATGEODEC:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATCLUMPSRA:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATCLUMPSDEC:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATCLUMPSGEORA:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATCLUMPSGEODEC:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATBRIGHTNESS:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATCLUMPSBRIGHTNESS:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATNORIVERBRIGHTNESS:
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATMAGNITUDE:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATMAGNITUDEERR:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATCLUMPSMAGNITUDE:
          p->objcols[p->objncols++] = p->allcols[i];
          break;
        case CATRIVERAVE:
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATRIVERNUM:
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATSN:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATSKY:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATSTD:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATSEMIMAJOR:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATSEMIMINOR:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATPOSITIONANGLE:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATGEOSEMIMAJOR:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATGEOSEMIMINOR:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        case CATGEOPOSITIONANGLE:
          p->objcols[p->objncols++] = p->allcols[i];
          p->clumpcols[p->clumpncols++] = p->allcols[i];
          break;
        default:
          error(EXIT_FAILURE, 0, "a bug! Please contact us at %s so we "
                "can fix the problem. For some reason p->allcols[%lu] "
                "is given the value %lu which is not recognized in "
                "preparearrays (ui.c)", PACKAGE_BUGREPORT, i,
                p->allcols[i]);
        }
    }


  /* Read the input image. Note that after this step, everything
     depends on having an input filename. If the user just wants to
     check the parameters, there is no input file name. */
  if(p->up.inputname)
    {
      gal_fits_file_to_float(p->up.inputname, p->up.maskname, p->cp.hdu,
                                  p->up.mhdu, &p->img, &bitpix, &anyblank,
                                  &p->s0, &p->s1);
      gal_fits_read_wcs(p->up.inputname, p->cp.hdu, 0, 0, &p->nwcs,
                        &p->wcs);


      /* Read and check the object, sky and skystd HDUs. Note that the
         clumps image is only used when the objects image says a clumps
         image exists. */
      checksetfloat(p, p->up.skyname, p->up.skyhdu, &p->sky);
      checksetfloat(p, p->up.stdname, p->up.stdhdu, &p->std);
      checksetlong(p, p->up.objlabsname, p->up.objhdu, &p->objects);
      if(p->up.clumplabsname)
        checksetlong(p, p->up.clumplabsname, p->up.clumphdu, &p->clumps);
      else
        p->clumps=NULL;

      /* Read the necessary keywords. */
      readkeywords(p);

      /* Allocate the catalog arrays: */
      if(p->objncols>0 && p->numobjects>0)
        {
          errno=0;
          p->objcat=malloc(p->objncols*p->numobjects*sizeof *p->objcat);
          if(p->objcat==NULL)
            error(EXIT_FAILURE, errno, "%lu bytes for p->objcat in "
                  "preprarearrays (ui.c)",
                  p->objncols*p->numobjects*sizeof *p->objcat);
        }
      else p->objcat=NULL;
      if(p->clumpncols>0 && p->numclumps>0)
        {
          errno=0;
          p->clumpcat=malloc(p->clumpncols*p->numclumps
                             * sizeof *p->clumpcat);
          if(p->clumpcat==NULL)
            error(EXIT_FAILURE, errno, "%lu bytes for p->clumpcat in "
                  "preprarearrays (ui.c)",
                  p->clumpncols*p->numclumps*sizeof *p->clumpcat);
        }
      else p->clumpcat=NULL;
    }


  /* Allocate two arrays to keep all the basic information about each
     object and clump. Note that there should be one row more than the
     total number of objects or clumps. This is because we want each
     label to be its row number and we don't have any object label of
     zero.*/
  errno=0; p->oinfo=calloc(OCOLUMNS*(p->numobjects+1), sizeof *p->oinfo);
  if(p->oinfo==NULL)
    error(EXIT_FAILURE, errno, "%lu bytes for p->oinfo in mkcatalog "
          "(mkcatalog.c)", OCOLUMNS*(p->numobjects+1)*sizeof *p->oinfo);
  errno=0; p->cinfo=calloc(CCOLUMNS*(p->numclumps+1), sizeof *p->cinfo);
  if(p->cinfo==NULL)
    error(EXIT_FAILURE, errno, "%lu bytes for p->cinfo in mkcatalog "
          "(mkcatalog.c)", CCOLUMNS*(p->numclumps+1)*sizeof *p->cinfo);

  /* Set the shift values to NAN for all the objects and clumps: */
  for(i=1;i<=p->numobjects;++i)
    p->oinfo[i*OCOLUMNS+OPOSSHIFTX]=p->oinfo[i*OCOLUMNS+OPOSSHIFTY]=NAN;
  for(i=1;i<=p->numclumps;++i)
    p->cinfo[i*CCOLUMNS+CPOSSHIFTX]=p->cinfo[i*CCOLUMNS+CPOSSHIFTY]=NAN;

  /* Clean up: */
  gal_linkedlist_free_sll(p->allcolsll);
}



















/**************************************************************/
/************         Set the parameters          *************/
/**************************************************************/
void
setparams(int argc, char *argv[], struct mkcatalogparams *p)
{
  struct gal_commonparams *cp=&p->cp;

  /* Set the non-zero initial values, the structure was initialized to
     have a zero value for all elements. */
  cp->spack         = SPACK;
  cp->verb          = 1;
  cp->numthreads    = num_processors(NPROC_CURRENT);
  cp->removedirinfo = 1;

  p->threshold      = NAN;

  /* Read the arguments. */
  errno=0;
  if(argp_parse(&thisargp, argc, argv, 0, 0, p))
    error(EXIT_FAILURE, errno, "parsing arguments");


  /* Add the user default values and save them if asked. */
  GAL_CONFIGFILES_CHECK_SET_CONFIG;

  /* Check if all the required parameters are set. */
  checkifset(p);

  /* Do a sanity check. Note that if the user just wants to see the
     parameters and hasn't given any file name, sanity check is
     useless, because in MakeProfiles, sanitycheck just checks the
     file names. So we first have to check if an input */
  if(p->up.inputname)
    sanitycheck(p);


  /* Make the array of input images. */
  preparearrays(p);

  /* Print the values for each parameter. */
  if(cp->printparams)
    GAL_CONFIGFILES_REPORT_PARAMETERS_SET;

  /* Everything is ready, notify the user of the program starting. */
  if(cp->verb)
    {
      printf(SPACK_NAME" started on %s", ctime(&p->rawtime));
      printf("  - Input   %s (hdu: %s)\n", p->up.inputname, p->cp.hdu);
      if(p->up.masknameset)
        printf("  - Mask   %s (hdu: %s)\n", p->up.maskname, p->up.mhdu);
      printf("  - Objects %s (hdu: %s)\n", p->up.objlabsname,
             p->up.objhdu);
      if(p->up.clumplabsname)
        printf("  - Clumps  %s (hdu: %s)\n", p->up.clumplabsname,
               p->up.clumphdu);
      printf("  - Sky     %s (hdu: %s)\n", p->up.skyname, p->up.skyhdu);
      printf("  - Sky STD %s (hdu: %s)\n", p->up.stdname, p->up.stdhdu);
    }
}




















/**************************************************************/
/************      Free allocated, report         *************/
/**************************************************************/
void
freeandreport(struct mkcatalogparams *p, struct timeval *t1)
{
  /* Free all the allocated spaces: */
  free(p->sky);
  free(p->std);
  free(p->objcat);
  free(p->cp.hdu);
  free(p->clumps);
  free(p->allcols);
  free(p->objects);
  free(p->objcols);
  free(p->clumpcat);
  free(p->ocatname);
  free(p->ccatname);
  free(p->clumpcols);
  free(p->up.objhdu);
  free(p->cp.output);
  free(p->up.skyhdu);
  free(p->up.stdhdu);
  free(p->up.clumphdu);
  if(p->up.mhduset) free(p->up.mhdu);
  if(p->wcs) wcsvfree(&p->nwcs, &p->wcs);
  if(p->up.skynameset) free(p->up.skyname);
  if(p->up.stdnameset) free(p->up.stdname);
  if(p->up.masknameset) free(p->up.maskname);
  if(p->up.objlabsnameset) free(p->up.objlabsname);
  if(p->up.clumplabsnameset) free(p->up.clumplabsname);

  /* Print the final message. */
  if(p->cp.verb)
    gal_timing_report(t1, SPACK_NAME" finished in", 0);
}