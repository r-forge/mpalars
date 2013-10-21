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

    Contact : quentin.grimonprez@inria.fr
*/

/*
 * Project:  MPAGenomics::
 * created on: 31 mai 2013
 * Author:   Quentin Grimonprez
 **/

/** @file LassoPenalty.cpp
 *  @brief In this file, declaration of the function of the class @c LassoPenalty.
 **/

#include "LassoPenalty.h"

namespace HD
{
  /*default cosntructor*/
  LassoPenalty::LassoPenalty()
                            : IPenalty()
                            , lambda_()
                            , sqrtInvPenalty_()
                            , sigma2_(1.)
  {
  }

  /* Constructor
  *  @param lambda penalization parameter for the l1-norm of the estimates
  *  @param n size of sample
  *  @param p size of Penalty (number of covariates)
  */
  LassoPenalty::LassoPenalty(STK::Real lambda)
                            : IPenalty()
                            , lambda_(lambda)
                            , sqrtInvPenalty_(0)
                            , sigma2_(1.)
  {
  }


  /*
   * Copy constructor
   * @param penalty LassoPenalty object to copy
   */
  LassoPenalty::LassoPenalty(LassoPenalty const& penalty)
              : IPenalty(penalty)
              , lambda_(penalty.lambda())
              , sqrtInvPenalty_(penalty.invPenalty())
              , sigma2_(penalty.sigma2())
  {
  }

  /*clone*/
  LassoPenalty* LassoPenalty::clone() const
  {
    return new LassoPenalty(*this);
  }


  /*
   * update sigma2 and the lasso penalty
   * @param beta current estimates
   * @param normResidual ||y-X*beta||_2^2
   */
  void LassoPenalty::update(STK::CVectorX const& beta, STK::Real const& normResidual)
  {
    updatePenalty(beta);
//    updateSigma2(beta,normResidual);
  }

  /*
   * update the lasso penalty (for fixed sigma)
   * @param beta current estimates
   */
  void LassoPenalty::update(STK::CVectorX const& beta)
  {
    updatePenalty(beta);
  }

  /*
   * @param x a vector of length p_
   * @return the product invPenalty_*x
   */
  STK::CVectorX LassoPenalty::multInvPenalty(STK::CVectorX const& x) const
  {
    STK::CVectorX a;
    a = sqrtInvPenalty_.square() * x;

    return a;
  }

  /*
   * @param x a vector of length p_
   * @return the product invPenalty_.sqrt()*x
   */
  STK::CVectorX LassoPenalty::multSqrtInvPenalty(STK::CVectorX const& x) const
  {
    STK::CVectorX a;
    a = sqrtInvPenalty_ * x;

    return a;
  }


  /* penalty term
   *  @param beta current estimates
   *  @return t(beta) * penalty * beta
   */
  STK::Real LassoPenalty::penaltyTerm(STK::CVectorX const& beta) const
  {
    //t(beta) * penalty * beta = lambda_ * \sum_i beta[i]^2(k)/abs(beta[i](k-1))
    //STK::Real penaltyTerm=beta.abs().sum() * lambda_;
    STK::Real penaltyTerm= lambda_ * beta.dot( (sqrtInvPenalty_.square()).inverse() * beta);

    return penaltyTerm;
  }

  /*
   * update the penalty
   * @param beta current estimates
   */
  void LassoPenalty::updatePenalty(STK::CVectorX const& beta)
  {
    if(beta.sizeRows()!=sqrtInvPenalty_.sizeRows())
      sqrtInvPenalty_.resize(STK::Range(1,beta.sizeRows()),STK::Range(1,beta.sizeRows()));

    for(int i = 1; i <= sqrtInvPenalty_.sizeRows(); i++)
      sqrtInvPenalty_[i] = std::sqrt(std::abs(beta[i]) / lambda_);
  }

  /*
   * update sigma2
   * @param beta current estimates
   * @param normResidual ||y-X*beta||_2^2
   */
//  void LassoPenalty::updateSigma2(STK::CVectorX const& beta, STK::Real const& normResidual)
//  {
//    //normbeta = t(beta) * invD * beta
//    //invD = matrix (1/tau_i^2, i=1,...,p)
//    //E[1/tau_i^2 | ] = lambda/|beta_i|
//    STK::Real normBeta=beta.abs().sum();
//    normBeta *= lambda_;
//    sigma2_ = (normResidual + normBeta) / (n_+p_-1.);
//  }

}
