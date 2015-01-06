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
#ifndef WCSMODE_H
#define WCSMODE_H

void
wcscheckprepare(struct imgcropparams *p, struct inputimgs *img);

void
setcsides(struct cropparams *crp);

int
radecoverlap(struct cropparams *crp);

#endif