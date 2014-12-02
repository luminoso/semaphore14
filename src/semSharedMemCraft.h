/**
 *  \file semSharedMemCraft.h (interface file)
 *
 *  \brief Problem name: Aveiro Handicraft SARL.
 *
 *  \brief Concept: Pedro Mariano.
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  Definition of the operations carried out by the craftsmen:
 *     \li collectMaterials
 *     \li primeMaterialsNeed
 *     \li backToWork
 *     \li prepareToProduce
 *     \li goToStore
 *     \li batchReadyForTransfer
 *     \li endOperCraftsman.
 *
 *  \author António Rui Borges - October 2014
 */

#ifndef SEMSHAREDMEMCRAFT_H_
#define SEMSHAREDMEMCRAFT_H_

#include <stdbool.h>

/**
 *  \brief Collect materials operation.
 *
 *  The craftsman gets the prime materials he needs to manufacture a product.
 *
 *  \param craftId identification of the craftsman
 *
 *  \return -c true, if it is necessary to phone the entrepreneur to let her know the workshop requires more prime materials
 *  \return -c false, otherwise
 */

extern bool collectMaterials (unsigned int craftId);

/**
 *  \brief Prime materials needed operation.
 *
 *  The craftsman phones the entrepreneur to let her know the workshop requires more prime materials.
 *
 *  \param craftId identification of the craftsman
 */

extern void primeMaterialsNeeded (unsigned int craftId);

/**
 *  \brief Back to work operation.
 *
 *  The craftsman returns to his regular duties.
 *
 *  \param craftId identification of the craftsman
 */

extern void backToWork (unsigned int craftId);

/**
 *  \brief Prepare to produce operation.
 *
 *  The craftsman sits down and prepares things for the production of a new piece.
 *
 *  \param craftId identification of the craftsman
 */

extern void prepareToProduce (unsigned int craftId);

/**
 *  \brief Go to store operation.
 *
 *  The craftsman stores the finished product.
 *
 *  \param craftId identification of the craftsman
 *
 *  \return number of products presently stored in the storeroom
 */

extern unsigned int goToStore (unsigned int craftId);

/**
 *  \brief Batch ready for transfer operation.
 *
 *  The craftsman phones the entrepreneur to let her know she should collect a batch of goods.
 *
 *  \param craftId identification of the craftsman
 */

extern void batchReadyForTransfer (unsigned int craftId);

/**
 *  \brief End of operations for the craftsman.
 *
 *  Checking the end of life cycle of the craftsman.
 *  The craftsman stops if all prime materials delivers have been carried out and if the amount of prime materials
 *  still remaining is less than the number of craftsmen still active times the amount of prime materials necessary
 *  to produce a piece.
 *  When the returned value is -c true for the last craftsman still alive, a check is made on the number of finished
 *  products in store. If the number is less then MAX, the entrepreneur is alerted to come and collect this last batch.
 *
 *  \param craftId identification of the craftsman
 *
 *  \return -c true, if the life cycle of the craftsman has come to an end
 *  \return -c false, otherwise
 */

extern bool endOperCraftsman (unsigned int craftId);

#endif /* SEMSHAREDMEMCRAFT_H_ */
