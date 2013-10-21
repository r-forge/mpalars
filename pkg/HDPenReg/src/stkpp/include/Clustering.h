/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004  Serge Iovleff

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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)                                  */

/*
 * Project:  stkpp::Clustering
 * Purpose:  Main include file for the Clustering project.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file Clustering.h
 *  @brief This file include all the header files of the project Clustering.
 *
 *  @defgroup Clustering Generative clustering
 *  @brief The project Clustering proposes classes for modeling and estimating
 *  generative mixture model.
 *
 *  The aim of this project is to define Interface and specialized classes
 *  in order to manipulate and estimate the parameters of any kind of
 *  generative mixture model.
 *
 *  In statistics, a mixture model is a probabilistic model for representing
 *  the presence of subpopulations within an overall population, without
 *  requiring that an observed data-set should identify the sub-population to
 *  which an individual observation belongs. Formally a mixture model
 *  corresponds to the mixture distribution that represents the probability
 *  distribution of observations in the overall population. However, while
 *  problems associated with "mixture distributions" relate to deriving the
 *  properties of the overall population from those of the sub-populations,
 *  "mixture models" are used to make statistical inferences about the
 *  properties of the sub-populations given only observations on the pooled
 *  population, without sub-population-identity information.
 *
 *  Some ways of implementing mixture models involve steps that attribute
 *  postulated sub-population-identities to individual observations (or weights
 *  towards such sub-populations), in which case these can be regarded as types
 *  of unsupervised learning or clustering procedures. However not all inference
 *  procedures involve such steps.
 **/

#ifndef CLUSTERING_H
#define CLUSTERING_H

#include "../projects/Clustering/include/STK_Clust_Util.h"
#include "../projects/Clustering/include/STK_MixtureFacade.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ajk_bjk.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ajk_bj.h"

#endif // CLUSTERING_H