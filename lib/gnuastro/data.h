/*********************************************************************
data -- Structure and functions to represent/work with data
This is part of GNU Astronomy Utilities (Gnuastro) package.

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
#ifndef __GAL_DATA_H__
#define __GAL_DATA_H__

/* Include other headers if necessary here. Note that other header files
   must be included before the C++ preparations below */
#include <math.h>
#include <limits.h>
#include <stdint.h>

#include <fitsio.h>             /* Only for the LONGLONG type */
#include <wcslib/wcs.h>
#include <gsl/gsl_complex.h>

/* When we are within Gnuastro's building process, `IN_GNUASTRO_BUILD' is
   defined. In the build process, installation information (in particular
   `GAL_CONFIG_ARITH_CHAR' and the rest of the types that we needed in the
   arithmetic function) is kept in `config.h'. When building a user's
   programs, this information is kept in `gnuastro/config.h'. Note that all
   `.c' files must start with the inclusion of `config.h' and that
   `gnuastro/config.h' is only created at installation time (not present
   during the building of Gnuastro).*/
#ifndef IN_GNUASTRO_BUILD
#include <gnuastro/config.h>
#endif

/* C++ Preparations */
#undef __BEGIN_C_DECLS
#undef __END_C_DECLS
#ifdef __cplusplus
# define __BEGIN_C_DECLS extern "C" {
# define __END_C_DECLS }
#else
# define __BEGIN_C_DECLS                /* empty */
# define __END_C_DECLS                  /* empty */
#endif
/* End of C++ preparations */



/* Actual header contants (the above were for the Pre-processor). */
__BEGIN_C_DECLS  /* From C++ preparations */





/* Macros: */

/* If this macro is 1, the only native type for arithmetic will be
   float. Having the four different types set as native can greatly
   lengthen the compilation time and slow-down the
   debugging/developement. */
#define GAL_DATA_ARITH_ONLY_FLOAT_FOR_FAST_DEBUG 0

/* The maximum dimensionality of datasets. */
#define GAL_DATA_MAXDIM    999

/* Arithmetic macros (powers of 2). */
#define GAL_DATA_ARITH_INPLACE  1
#define GAL_DATA_ARITH_FREE     2
#define GAL_DATA_ARITH_NUMOK    4

/* Blank values: Note that for the unsigned types or small types (like
   char), the maximum value is considered as a blank value, since the
   minimum value of an unsigned type is zero and zero is often meaningful
   in contexts were unsigned values are used. */
#define GAL_DATA_BLANK_UCHAR      UCHAR_MAX
#define GAL_DATA_BLANK_CHAR       SCHAR_MAX
#define GAL_DATA_BLANK_LOGICAL    SCHAR_MAX
#define GAL_DATA_BLANK_STRING     NULL
#define GAL_DATA_BLANK_USHORT     USHRT_MAX
#define GAL_DATA_BLANK_SHORT      INT16_MIN
#define GAL_DATA_BLANK_UINT       UINT_MAX
#define GAL_DATA_BLANK_INT        INT_MIN
#define GAL_DATA_BLANK_ULONG      ULONG_MAX
#define GAL_DATA_BLANK_LONG       INT32_MIN
#define GAL_DATA_BLANK_LONGLONG   INT64_MIN
#define GAL_DATA_BLANK_FLOAT      NAN
#define GAL_DATA_BLANK_DOUBLE     NAN





/* Macros to identify the type of data. The macros in the comment
   parenthesis is the equivalent macro in CFITSIO. */
enum gal_data_alltypes
{
  GAL_DATA_TYPE_BIT,       /* Bit              (TBIT).        */
  GAL_DATA_TYPE_UCHAR,     /* unsigned char    (TBYTE).       */
  GAL_DATA_TYPE_CHAR,      /* char             (TSBYTE).      */
  GAL_DATA_TYPE_LOGICAL,   /* char             (TLOGICAL).    */
  GAL_DATA_TYPE_STRING,    /* string           (TSTRING).     */
  GAL_DATA_TYPE_USHORT,    /* unsigned short   (TUSHORT).     */
  GAL_DATA_TYPE_SHORT,     /* short            (TSHORT).      */
  GAL_DATA_TYPE_UINT,      /* unsigned int     (TUINT).       */
  GAL_DATA_TYPE_INT,       /* int              (TINT).        */
  GAL_DATA_TYPE_ULONG,     /* unsigned long    (TLONG).       */
  GAL_DATA_TYPE_LONG,      /* long             (TLONG).       */
  GAL_DATA_TYPE_LONGLONG,  /* long long        (TLONGLONG).   */
  GAL_DATA_TYPE_FLOAT,     /* float            (TFLOAT).      */
  GAL_DATA_TYPE_DOUBLE,    /* double           (TDOUBLE).     */
  GAL_DATA_TYPE_COMPLEX,   /* Complex float    (TCOMPLEX).    */
  GAL_DATA_TYPE_DCOMPLEX,  /* Complex double   (TDBLCOMPLEX). */
};





enum gal_data_operators
{
  GAL_DATA_OPERATOR_PLUS,      /*   +    */
  GAL_DATA_OPERATOR_MINUS,     /*   -    */
  GAL_DATA_OPERATOR_MULTIPLY,  /*   *    */
  GAL_DATA_OPERATOR_DIVIDE,    /*   /    */

  GAL_DATA_OPERATOR_LT,        /*   <    */
  GAL_DATA_OPERATOR_LE,        /*   <=   */
  GAL_DATA_OPERATOR_GT,        /*   >    */
  GAL_DATA_OPERATOR_GE,        /*   >=   */
  GAL_DATA_OPERATOR_EQ,        /*   ==   */
  GAL_DATA_OPERATOR_NE,        /*   !=   */
  GAL_DATA_OPERATOR_AND,       /*   &&   */
  GAL_DATA_OPERATOR_OR,        /*   ||   */
  GAL_DATA_OPERATOR_BITAND,    /*   &    */
  GAL_DATA_OPERATOR_BITOR,     /*   |    */

  GAL_DATA_OPERATOR_NOT,       /*   !    */
  GAL_DATA_OPERATOR_ISBLANK,   /* Similar to isnan() for floats. */
  GAL_DATA_OPERATOR_WHERE,     /*   ?:   */

  GAL_DATA_OPERATOR_ABS,       /* abs()  */
  GAL_DATA_OPERATOR_POW,       /* pow()  */
  GAL_DATA_OPERATOR_SQRT,      /* sqrt() */
  GAL_DATA_OPERATOR_LOG,       /* log()  */
  GAL_DATA_OPERATOR_LOG10,     /* log()  */

  GAL_DATA_OPERATOR_MINVAL,    /* Minimum value of array.                */
  GAL_DATA_OPERATOR_MAXVAL,    /* Maximum value of array.                */
  GAL_DATA_OPERATOR_MIN,       /* Minimum per pixel of multiple arrays.  */
  GAL_DATA_OPERATOR_MAX,       /* Maximum per pixel of multiple arrays.  */
  GAL_DATA_OPERATOR_AVERAGE,   /* Average per pixel of multiple arrays.  */
  GAL_DATA_OPERATOR_MEDIAN,    /* Median per pixel of multiple arrays.   */
};





/* Main data structure.

   Notes
   -----

    - If mmapname==NULL, then the array is allocated (using malloc, in the
      RAM), otherwise its is mmap'd (is actually a file on the ssd/hdd).

    - minmapsize is stored in the data structure to allow any derivative
      data structures to follow the same number and decide if they should
      be mmap'd or allocated.

      - `minmapsize' ==0:  array is definitely mmap'd.

      - `minmapsize' ==-1: array is definitely in RAM.

    - The `dsize' array is in the `long' type because CFITSIO uses the long
      type and this will make it easier to call CFITSIO functions.*/
typedef struct
{
  void        *array;  /* Array keeping data elements.               */
  int           type;  /* Type of data (from `gal_data_alltypes').   */
  size_t        ndim;  /* Number of dimensions in the array.         */
  long        *dsize;  /* Size of array along each dimension.        */
  size_t        size;  /* Total number of data-elements.             */
  char     *mmapname;  /* File name of the mmap.                     */
  size_t  minmapsize;  /* Minimum number of bytes to mmap the array. */
  int       anyblank;  /* ==1: has blank values.                     */
  int           nwcs;  /* for WCSLIB: no. coord. representations.    */
  struct wcsprm *wcs;  /* WCS information for this dataset.          */
} gal_data_t;





/*********************************************************************/
/*************         Size and allocation         *******************/
/*********************************************************************/
int
gal_data_dsize_is_different(gal_data_t *first, gal_data_t *second);

size_t
gal_data_sizeof(int type);

void *
gal_data_malloc_array(int type, size_t size);

void *
gal_data_calloc_array(int type, size_t size);

gal_data_t *
gal_data_alloc(void *array, int type, size_t ndim, long *dsize,
               struct wcsprm *wcs, int clear, size_t minmapsize);

void
gal_data_free(gal_data_t *data);






/*************************************************************
 **************          Blank data            ***************
 *************************************************************/
void *
gal_data_alloc_blank(int type);

void
gal_data_apply_mask(gal_data_t *in, gal_data_t *mask);

void
gal_data_blank_to_value(gal_data_t *data, void *value);





/*************************************************************
 **************       Types and copying        ***************
 *************************************************************/
char *
gal_data_type_string(int type);

gal_data_t *
gal_data_copy(gal_data_t *in);

gal_data_t *
gal_data_copy_to_new_type(gal_data_t *in, int newtype);

int
gal_data_out_type(gal_data_t *first, gal_data_t *second);

void
gal_data_to_same_type(gal_data_t *f, gal_data_t *s,
                      gal_data_t **of, gal_data_t **os,
                      int type, int freeinputs);





/*************************************************************
 **************              Read              ***************
 *************************************************************/
gal_data_t *
gal_data_string_to_number(char *string);





/*************************************************************
 **************           Arithmetic           ***************
 *************************************************************/
gal_data_t *
gal_data_arithmetic(int operator, unsigned char flags, ...);





__END_C_DECLS    /* From C++ preparations */

#endif           /* __GAL_BOX_H__ */