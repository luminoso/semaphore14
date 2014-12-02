/**
 *  \file sharedDataSync.h (interface file)
 *
 *  Problem name: Aveiro Handicraft SARL
 *
 *  Concept: Pedro Mariano
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  \brief Definition of the shared data and the synchronization devices.
 *
 *  Both the format of the shared data, which represents the full state of the problem, and the identification of
 *  the different semaphores, which carry out the synchronization among the intervening entities, are provided.
 *
 *  \author António Rui Borges - October 2014
 */

#ifndef SHAREDDATASYNC_H_
#define SHAREDDATASYNC_H_

#include "probConst.h"
#include "probDataStruct.h"

/**
 *  \brief Definition of <em>shared information</em> data type.
 */
typedef struct
        { /** \brief full state of the problem */
          FULL_STAT fSt;
          /** \brief identification of critical region protection semaphore – val = 1 */
          unsigned int access;
          /** \brief identification of entrepreneur appraise situation semaphore – val = 0 */
          unsigned int proceed;
          /** \brief identification of customers waiting for service semaphore array – val = 0 (one per customer) */
          unsigned int waitForService[N];
          /** \brief identification of craftsmen waiting for prime materials semaphore – val = 0 */
          unsigned int waitForMaterials;
          /** \brief number of craftsmen who are blocked waiting for the availability of prime materials */
          unsigned int nCraftsmenBlk;
        } SHARED_DATA;

/** \brief number of semaphores in the set */
#define SEM_NU                 (N+3)

/** \brief index of critical region protection semaphore */
#define ACCESS                     1

/** \brief index of entrepreneur appraise situation semaphore */
#define PROCEED                    2

/** \brief index of identification of craftsmen waiting for prime materials semaphore */
#define WAITFORMATERIALS           3

/** \brief base index of customers waiting for service semaphore array (one per customer) */
#define B_WAITFORSERVICE           4

#endif /* SHAREDDATASYNC_H_ */
