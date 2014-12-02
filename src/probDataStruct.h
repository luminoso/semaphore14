/**
 *  \file probDataStruct.h (interface file)
 *
 *  \brief Problem name: Aveiro Handicraft SARL.
 *
 *  \brief Concept: Pedro Mariano.
 *
 *  Definition of internal data structures.
 *
 *  They specify internal metadata about the status of the intervening entities.
 *
 *  \author António Rui Borges - October 2014
 */

#ifndef PROBDATASTRUCT_H_
#define PROBDATASTRUCT_H_

#include <stdbool.h>

#include "probConst.h"

/**
 *  \brief Definition of <em>state of the customers</em> data type.
 */
typedef struct
        { /** \brief internal state */
          unsigned int stat;
          /** \brief amount of pieces bought so far */
          unsigned int boughtPieces;
          /** \brief availability flag required by the simulation:
           *         \li \c true - customer is active
           *         \li \c false - otherwise */
          bool readyToWork;
        } STAT_CUST;

/**
 *  \brief Definition of <em>state of the craftmen</em> data type.
 */
typedef struct
        { /** \brief internal state */
          unsigned int stat;
          /** \brief amount of pieces produced so far */
          unsigned int prodPieces;
          /** \brief availability flag required by the simulation:
           *         \li \c true - crafstman is active
           *         \li \c false - otherwise */
          bool readyToWork;
        } STAT_CRAFT;

/**
 *  \brief Definition of <em>state of the intervening entities</em> data type.
 */
typedef struct
        { /** \brief entrepreneur state */
          unsigned int entrepStat;
          /** \brief customers state array */
          STAT_CUST custStat[N];
          /** \brief craftsmen state array */
          STAT_CRAFT craftStat[M];
        } STAT;

/** \brief queue position is empty */
#define  EMPTYPOS    ((unsigned int) -1)

/**
 *  \brief Definition of <em>waiting queue</em> data type.
 */
typedef struct
        { /** \brief storage region */
          unsigned int mem[N];
          /** \brief insertion pointer */
          unsigned int ii;
          /** \brief retrieval pointer */
          unsigned int ri;
          /** \brief flag signalling that the queue is full */
          bool full;
        } QUEUE;

/** \brief the shop is open */
#define  SOPEN           0
/** \brief the shop door is closed */
#define  SDCLOSED        1
/** \brief the shop is closed */
#define  SCLOSED         2

/**
 *  \brief Definition of <em>state of the shop</em> data type.
 */
typedef struct
        { /** \brief shop status: either SOPEN, or SDCLOSED, or SCLOSED */
          unsigned int stat;
          /** \brief number of customers in the shop */
          unsigned int nCustIn;
          /** \brief number of products in the shop */
          unsigned int nProdIn;
          /** \brief flag signaling a craftsman has phoned the entrepreneur requesting the transfer of a new batch of products */
          bool prodTransfer;
          /** \brief flag signaling a craftsman has phoned the entrepreneur asking for the deliver of more prime materials */
          bool primeMatReq;
          /** \brief queue by the counter formed by the customers which want to buy goods */
          QUEUE queue;
        } SHOPINFO;

/**
 *  \brief Definition of <em>state of the workshop</em> data type.
 */
typedef struct
        { /** \brief amount of prime materials in the workshop */
          unsigned int nPMatIn;
          /** \brief number of finished products in the storeroom */
          unsigned int nProdIn;
          /** \brief number of times prime materials have been supplied */
          unsigned int NSPMat;
          /** \brief total amount of prime materials supplied */
          unsigned int NTPMat;
          /** \brief total number of pieces produced */
          unsigned int NTProd;
        } WORKSHOPINFO;

/**
 *  \brief Definition of <em>full state of the problem</em> data type.
 */
typedef struct
        { /** \brief state of all intervening entities */
          STAT st;
          /** \brief state of the shop */
          SHOPINFO shop;
          /** \brief state of the workshop */
          WORKSHOPINFO workShop;
          /** \brief amount of prime materials supplied each time */
          unsigned int primeMaterials[NP];
        } FULL_STAT;

#endif /* PROBDATASTRUCT_H_ */
