/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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

 Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/*
 * Project:  stkpp::Arrays
 * created on: 27 sept. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ArrayBaseVisitor.h
 *  @brief In this file we define the ArrayBaseVisitor classes.
 **/

#ifndef STK_VISITORS_H
#define STK_VISITORS_H

#include "../../../STKernel/include/STK_MetaTemplate.h"
#include "../STK_Arrays_Util.h"

namespace STK
{

namespace hidden
{

/** @ingroup hidden
  * @brief utility class for getting the result from a visitor acting on a
  * vector or a point.
  */
template <class Visitor, int Structure_> struct GetIdx;
template <class Visitor> struct GetIdx<Visitor, Arrays::vector_>
{ inline static int idx(Visitor const& visitor) { return visitor.row_;};};

template <class Visitor> struct GetIdx<Visitor, Arrays::point_>
{ inline static int idx(Visitor const& visitor) { return visitor.col_;};};

template <class Visitor> struct GetIdx<Visitor, Arrays::diagonal_>
{ inline static int idx(Visitor const& visitor) { return visitor.col_;};};


/** @ingroup hidden
  * @brief Base class to implement min, max, sum,... visitors for 2D containers.
  */
template <typename Type>
struct EltVisitor2DBase
{
  EltVisitor2DBase() : row_(STKBASEARRAYS), col_(STKBASEARRAYS), res_(Arithmetic<Type>::NA()) {};
  int row_, col_;
  Type res_;
};

/** @ingroup hidden
  * @brief Visitor computing the min coefficient with its value and coordinates
  *
  * @sa ArrayBase::minElt(int, int)
  */
template <typename Type>
struct MinEltVisitor : EltVisitor2DBase<Type>
{
  MinEltVisitor() : EltVisitor2DBase<Type>()
  { this->res_ = Arithmetic<Type>::max(); }
  void operator()( Type const& value, int i, int j)
  {
    if(value < this->res_)
    { this->res_ = value; this->row_ = i; this->col_ = j;}
  }
};

/** @ingroup hidden
  * @brief Visitor computing safely the min coefficient with its value and
  * coordinates
  *
  * @sa ArrayBase::minElt(int, int)
  */
template <typename Type>
struct MinEltSafeVisitor : EltVisitor2DBase<Type>
{
  MinEltSafeVisitor() : EltVisitor2DBase<Type>()
  { this->res_ = Arithmetic<Type>::max(); }
  void operator()( Type const& value, int i, int j)
  {
    if (Arithmetic<Type>::isFinite(value))
      if (value < this->res_)
      { this->res_ = value; this->row_ = i; this->col_ = j;}
  }
};

/** @ingroup hidden
  * @brief Visitor computing safely the min coefficient with its value and
  * coordinates
  *
  * @sa ArrayBase::minElt(int, int)
  */
/** @ingroup hidden
 *  @brief Visitor computing the maximal coefficient of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct MaxEltVisitor : EltVisitor2DBase<Type>
{
  MaxEltVisitor(): EltVisitor2DBase<Type>()
  { this->res_ = -Arithmetic<Type>::max(); }
  void operator() ( Type const& value, int i, int j)
  {
    if(value > this->res_)
    { this->res_ = value; this->row_ = i; this->col_ = j;}
  }
};
/** @ingroup hidden
 *  @brief Visitor computing safely the maximal coefficient of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct MaxEltSafeVisitor : EltVisitor2DBase<Type>
{
  MaxEltSafeVisitor(): EltVisitor2DBase<Type>()
  { this->res_ = -Arithmetic<Type>::max(); }
  void operator() ( Type const& value, int i, int j)
  {
    if (Arithmetic<Type>::isFinite(value))
      if(value > this->res_)
      { this->res_ = value; this->row_ = i; this->col_ = j;}
  }
};
/** @ingroup hidden
 *  @brief Visitor computing the sum of all the coefficients of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct SumVisitor
{
  Type res_;
  SumVisitor(): res_(Type(0)) {}
  void operator() ( Type const& value, int i, int j)
  { res_ += value;}
  void operator() ( Type const& value, int i)
  { res_ += value;}
};

/** @ingroup hidden
 *  @brief Visitor computing the sum of all the coefficients of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct SumSafeVisitor
{
  Type res_;
  SumSafeVisitor(): res_(Type(0)) {}
  void operator() ( Type const& value, int i, int j)
  { if (Arithmetic<Type>::isFinite(value)) res_ += value;}
  void operator() ( Type const& value, int i)
  { if (Arithmetic<Type>::isFinite(value)) res_ += value;}
};

/** @ingroup hidden
 *  @brief Visitor computing the sum of all the coefficients of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct RandUnifVisitor
{
  RandBase ran;
  RandUnifVisitor() : ran() {}
  void operator() ( Type& value)
  { value = Type(ran.randUnif());}
};

/** @ingroup hidden
 *  @brief Visitor computing the sum of all the coefficients of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct RandGaussVisitor
{
  RandBase ran;
  RandGaussVisitor() : ran() {}
  void operator() ( Type& value)
  { value = Type(ran.randGauss());}
};

/** @ingroup hidden
  * @brief visitor putting a constant value
  */
template <typename Type>
struct ValueVisitor
{
  Type value_;
  ValueVisitor(Type const& value) : value_(value) {};
  inline void operator() ( Type& value)
  { value = value_;}
};

} //namespace hidden

} // namespace STK

#endif /* STK_VISITORS_H */