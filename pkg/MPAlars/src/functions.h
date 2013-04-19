/*--------------------------------------------------------------------*/
/*     Copyright (C) 2013-2013  Serge Iovleff, Quentin Grimonprez

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  MPAGenomics::
 * created on: 6 févr. 2013
 * Author:   Quentin Grimonprez
 **/

/** @file functions.h
 *  @brief Contains utilities functions for lars algorithm.
 **/

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#include "PathState.h"

/**
 * (x1,y1) and (x2,y2) are points of an affine function, we wants to calculate the image of x3.
 * @param x1 abscissa of the first point
 * @param x2 abscissa of the second point
 * @param x3 abscissa of the point we want ordinate
 * @param y1 ordinate of the first point
 * @param y2 ordinate of the second point
 * @return ordinate of x3
 */
Real computeOrdinate(Real x1,Real x2,Real x3,Real y1,Real y2);

/**
 * Compute the coefficients for a given value of lambda
 * @param state1 state of a lars step
 * @param state2 state of the next lars step
 * @param evolution difference between the 2 lars step
 * @param lambda abscissa to compute ordinates
 * @return value of coefficients for lambda
 */
STK::Array2DVector< std::pair<int,Real> > computeCoefficients(MPA::PathState const& state1,MPA::PathState const& state2,std::pair<int,int> const& evolution, Real const& lambda);

void print(STK::Array2DVector< std::pair<int,Real> > const& state);
void import(std::string adressFichier,int n,int p,STK::CArrayXX &data);
void import(std::string adressFichier,int n,STK::CVectorX &data);

#endif /* FUNCTIONS_H_ */
