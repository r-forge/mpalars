/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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
 * created on: 25 nov. 2011
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_CArrayNumber.h
 *  @brief In this file we implement the final class CArrayNumber.
 **/

#ifndef STK_CARRAYNUMBER_H
#define STK_CARRAYNUMBER_H

#include "../../STKernel/include/STK_Constants.h"
#include "../../STKernel/include/STK_MetaTemplate.h"
#include "../../STKernel/include/STK_StaticAssert.h"

#include "../../Sdk/include/STK_Traits.h"

#include "STK_Arrays_Util.h"
#include "STK_CAllocator.h"
#include "STK_ICArray.h"

namespace STK
{
template< typename Type, int SizeRows_ =1, int SizeCols_=1, bool Orient_ = Arrays::by_col_>
class CArrayNumber;

template< typename Type, int SizeRows_, int SizeCols_, bool Orient_>
class CArray;
template< typename Type, int Size_, bool Orient_>
class CArraySquare;
template< typename Type, int SizeCols_, bool Orient_>
class CArrayPoint;
template< typename Type, int SizeRows_, int SizeCols_, bool Orient_ >
class CArrayVector;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for CArrayNumber class.
 */
template<typename Type_, int SizeRows_, int SizeCols_, bool Orient_>
struct Traits< CArrayNumber<Type_, SizeRows_, SizeCols_, Orient_> >
{
  private:
    class Void { };

    typedef CArrayNumber<Type_, 1, 1, Arrays::by_col_> RowIndirect;
    typedef CArrayNumber<Type_, 1, 1, Arrays::by_row_> RowDirect;

    typedef CArrayNumber<Type_, 1, 1, Arrays::by_col_> SubRowIndirect;
    typedef CArrayNumber<Type_, 1, 1, Arrays::by_row_> SubRowDirect;

    typedef CArrayNumber<Type_, 1, 1, Arrays::by_row_> ColIndirect;
    typedef CArrayNumber<Type_, SizeRows_, 1, Arrays::by_col_> ColDirect;

    typedef CArrayNumber<Type_, 1, 1, Arrays::by_row_> SubColIndirect;
    typedef CArrayNumber<Type_, 1, 1, Arrays::by_col_> SubColDirect;

    typedef CArrayNumber<Type_, 1, 1, Arrays::by_row_> FixedRowArrayIndirect;
    typedef CArrayNumber<Type_, 1, 1, Arrays::by_row_> FixedColArrayDirect;

    typedef CArrayNumber<Type_, 1, 1, Arrays::by_col_> FixedRowArrayDirect;
    typedef CArrayNumber<Type_, 1, 1, Arrays::by_col_> FixedColArrayIndirect;

  public:
    typedef CArrayNumber<Type_, 1, 1, Orient_> Number;

    typedef typename If<Orient_, RowIndirect, RowDirect >::Result  Row;
    typedef typename If<Orient_, ColDirect, ColIndirect >::Result  Col;

    typedef typename If<Orient_, SubRowIndirect, SubRowDirect >::Result  SubRow;
    typedef typename If<Orient_, SubColDirect, SubColIndirect >::Result  SubCol;

    /** If one of the Size is 1, we have a Vector (a column) or a Point (a row)
     *  (What to do if both are = 1 : Type or array (1,1) ?).
     **/
    typedef typename If< (SizeRows_ == 1)||(SizeCols_ == 1)   // one row or one column
                       , typename If<(SizeCols_ == 1), SubCol, SubRow>::Result
                       , Void
                       >::Result SubVector;
    // FIXME does not seem optimal if we want only to get a subset of rows (columns)
    typedef CArrayNumber<Type_, UnknownSize, UnknownSize, Orient_> SubArray;
//    typedef typename If< Orient_ == Arrays::by_col_
//                       , typename If<SizeRows_ != UnknownSize, FixedRowArrayDirect, FixedColArrayIndirect>::Result
//                       , typename If<SizeCols_ != UnknownSize, FixedRowArrayIndirect, FixedColArrayDirect>::Result
//                       >::Result SubArray;
    // Transposed type
    typedef CArrayNumber< Type_, SizeCols_, SizeRows_, !Orient_> Transposed;
    // The CAllocator have to have the same structure than the CArrayNumber
    typedef CAllocator<Type_, Arrays::number_, 1, 1, Orient_> Allocator;

    typedef Type_ Type;

    enum
    {
      structure_ = Arrays::number_,
      orient_    = Orient_,
      sizeRows_  = 1,
      sizeCols_  = 1,
      storage_   = Arrays::dense_
    };
};

} // namespace hidden


/** @ingroup Arrays
 * @brief specialization for the number case.
 */
template <typename Type, bool Orient_>
class CArrayNumber <Type, 1, 1, Orient_>
      : public ICArray < CArrayNumber<Type, 1, 1, Orient_> >
{
  public:
    typedef ICArray < CArrayNumber<Type, 1, 1, Orient_> > Base;
    typedef ArrayBase < CArrayNumber<Type, 1, 1, Orient_> > LowBase;
    enum
    {
      structure_ = Arrays::number_,
      orient_    = Orient_,
      sizeRows_  = 1,
      sizeCols_  = 1,
      storage_   = Arrays::dense_
    };
    /** Default constructor. */
    inline CArrayNumber() : Base() {}
    /** constructor with an initial value, initialization with a constant.
     *  @param v initial value of the container
     **/
    inline CArrayNumber( Type const& v) : Base(1, 1, v) {}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    inline CArrayNumber( const CArrayNumber &T, bool ref=false) : Base(T, ref) {}
    /** constructor by reference.
     *  @param allocator the allocator to wrap
     **/
    template<class OtherAllocator>
    inline CArrayNumber( OtherAllocator const& allocator) : Base(allocator) {}
    /** destructor. */
    inline ~CArrayNumber() {}
    /** operator= : set the container to a constant value.
     *  @param v the value to set
     **/
    inline CArrayNumber& operator=(Type const& v) { this->setValue(v); return *this;}
    /** operator = : overwrite the CArrayNumber with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline CArrayNumber& operator=(Rhs const& T) { return LowBase::assign(T);}
    /** operator = : overwrite the CArrayNumber with the Right hand side rhs.
     *  @param rhs the container to copy
     **/
    inline CArrayNumber& operator=(CArrayNumber const& rhs) { return LowBase::assign(rhs);}
};

} // namespace STK


#endif /* STK_CARRAYNUMBER_H */