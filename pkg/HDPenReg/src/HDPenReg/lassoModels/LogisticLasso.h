/*--------------------------------------------------------------------*/
/*     Copyright (C) 2013-2013  Serge Iovleff, Quentin Grimonprez

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public
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
 * created on: 4 nov. 2013
 * Author:   Quentin Grimonprez
 **/

/** @file LogisticLasso.h
 *  @brief In this file .
 **/


#ifndef LOGISTICLASSO_H_
#define LOGISTICLASSO_H_


#include "PenalizedModels.h"
#include "LogisticLassoPenalty.h"
#include "LogisticLassoSolver.h"

namespace HD
{
  class LogisticLasso;

  template<>
  struct ModelTraits<LogisticLasso>
  {
    typedef LogisticLassoSolver Solver;
    typedef LogisticLassoPenalty Penalty;
    typedef LassoMultiplicator Multiplicator;
    typedef STK::CG<LassoMultiplicator,STK::CVectorX, InitFunctor> CG;
  };

  /**
   * Class Lasso derived from @c PenalizedModels.
   * This class constructs a Lasso Model to be solved by an @c EM algorithm.
   */
  class LogisticLasso : public PenalizedModels<LogisticLasso>
  {
    public:
      /** default constructor*/
      LogisticLasso()
      : PenalizedModels<LogisticLasso>()
      {
        STK::CVectorX Xty(nbVar());

        // creation lasso penalty
        LogisticLassoPenalty*  p_penalty = new LogisticLassoPenalty();
        p_penalty_ = p_penalty;

        //creation functor for CG
        LassoMultiplicator mult(p_currentData(),p_penalty_->p_invPenalty(), p_penalty_->p_sigma2());
        mult_ = mult;

        //create CG
        STK::CG<LassoMultiplicator,STK::CVectorX, InitFunctor>* p_gcsolver = new STK::CG<LassoMultiplicator,STK::CVectorX,InitFunctor>(mult_, Xty);
        p_gcsolver_ = p_gcsolver;

        //create solver for lasso
        LogisticLassoSolver* p_lassosolver = new LogisticLassoSolver();
        p_lassosolver->setPenalty(p_penalty_);
        p_lassosolver->setSolver(p_gcsolver_);

        //add solver to lasso
        p_solver_ = p_lassosolver;

        //add pointer to currentData to the multiplicator functor
        mult_.p_data_= p_currentData();
      }

      /**
       * Constructor
       * @param p_data pointer to the data
       * @param p_y pointer to the response
       * @param lambda value of parameter associated to the l1 penalty
       * @param threshold threshold for setting coefficient to 0
       * @param epsCG epsilon for CG convergence
       */
      LogisticLasso(STK::CArrayXX const* p_data, STK::CVectorX const* p_y, STK::Real lambda, STK::Real threshold, STK::Real epsCG)
      : PenalizedModels<LogisticLasso>(p_data, p_y)
      {
        STK::CVectorX Xty(nbVar());
        Xty = p_data_->transpose() * *p_y_;

        STK::CVectorX beta0(Xty.sizeRows());
        for(int i = 1; i <= Xty.sizeRows();i++)
          beta0[i] = Xty[i]/((*p_data).col(i).norm2());

        beta_ = beta0;

        // creation lasso penalty
        LogisticLassoPenalty*  p_penalty = new LogisticLassoPenalty(lambda);
        p_penalty_ = p_penalty;
        p_penalty_->setPy(p_y_);

        //creation functor for CG
        LassoMultiplicator mult(p_currentData(),p_penalty_->p_invPenalty(), p_penalty_->p_sigma2());
        mult_ = mult;

        //create CG
        STK::CG<LassoMultiplicator,STK::CVectorX, InitFunctor>* p_gcsolver = new STK::CG<LassoMultiplicator,STK::CVectorX,InitFunctor>(mult_, Xty, 0, epsCG);
        p_gcsolver_ = p_gcsolver;

        //create solver for lasso
        LogisticLassoSolver* p_lassosolver = new LogisticLassoSolver(p_data_, beta_, p_penalty_->p_z(), threshold, p_gcsolver_, p_penalty_);

        //add the penalty
        p_lassosolver->setPenalty(p_penalty_);

        //add solver to lasso
        p_solver_ = p_lassosolver;

        p_penalty_->setPcurrentData(p_solver_->p_currentData());

//        InitFunctor init(p_solver_->p_currentBeta());
//        init_ = init;
//        (p_solver_->p_solver())->setInitFunctor(&init_);

        mult_.p_data_= p_currentData();
      }

      /** destructor*/
      ~LogisticLasso()
      {
        if(p_gcsolver_) delete p_gcsolver_;
      }

      /**
       * set the lasso regularization parameter
       * @param lambda
       */
      inline void setLambda(STK::Real lambda) {(p_solver_->p_penalty())->setLambda(lambda);}
      /**
       * set the threshold for the shrinkage
       * @param threshold
       */
      inline void setThreshold(STK::Real threshold) {p_solver_->setThreshold(threshold);}

      /**
       * set the epsilon for the CG
       * @param epsilon epsilon for the convergence of CG
       */
      inline void setCGEps(STK::Real eps) {p_gcsolver_->setEps(eps);}


      /**initialize the containers of all subclasses*/
      void initializeModel()
      {
        //compte intitial beta
        STK::CVectorX Xty(nbVar());
        Xty = p_data_->transpose() * *p_y_;

        STK::CVectorX beta0(Xty.sizeRows());
        for(int i = 1; i <= Xty.sizeRows();i++)
          beta0[i] = Xty[i]/((*p_data_).col(i).norm2());

        //add response to penalty
        p_penalty_->setPy(p_y_);

        beta_ = beta0;
        //set the parameter of the solver
        p_solver_->setData(p_data_);
        p_solver_->setY(p_penalty_->p_z());
        p_solver_->setBeta(beta_);
        //initialize the solver
        p_solver_->initializeSolver();
        //add currentData to penalty
        p_penalty_->setPcurrentData(p_solver_->p_currentData());

      }

      /**initialization of the class for with a new beta0
       * @param beta initial start for beta
       * */
      void initializeBeta(STK::CVectorX const& beta)
      {
        p_solver_->setBeta(beta_);
        p_solver_->initializeSolver();
      }

    private:
      /// multiplicator for conjugate gradient
      LassoMultiplicator mult_;
      /// conjugate gradient for solver
      STK::CG<LassoMultiplicator,STK::CVectorX, InitFunctor>* p_gcsolver_;
//      InitFunctor init_;
  };
}


#endif /* LOGISTICLASSO_H_ */
