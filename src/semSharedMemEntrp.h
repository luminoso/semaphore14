/**
 *  \file semSharedMemEntrp.h (interface file)
 *
 *  \brief Problem name: Aveiro Handicraft SARL.
 *
 *  \brief Concept: Pedro Mariano.
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  Definition of the operations carried out by the entrepreneur:
 *     \li prepareToWork
 *     \li appraiseSit
 *     \li addressACustomer
 *     \li sayGoodByeToCustomer
 *     \li customersInTheShop
 *     \li closeTheDoor
 *     \li prepareToLeave
 *     \li goToWorkShop
 *     \li visitSuppliers
 *     \li returnToShop
 *     \li endOperEntrep.
 *
 *  \author António Rui Borges - October 2014
 */

#ifndef SEMSHAREDMEMENTRP_H_
#define SEMSHAREDMEMENTRP_H_

#include <stdbool.h>

/**
 *  \brief Prepare to work operation.
 *
 *  The entrepreneur opens the shop and gets ready to perform her duties.
 */

extern void prepareToWork (void);

/**
 *  \brief Appraise situation operation.
 *
 *  The entrepreneur waits for service requests. She is waken up in the following cases:
 *    \li when a customer requests service at the counter
 *    \li when a customer exits the shop
 *    \li when a craftsman phones to request more prime materials for the workshop
 *    \li when a craftsman phones to ask for the collection of a new batch of products.
 *
 *  \return \c 'C', if a customer is needing attention
 *  \return \c 'P', if she should go shopping for prime materials
 *  \return \c 'G', if she should go to the workshop to collect a new batch of products
 *  \return \c 'E', if the simulation has come to an end
 */

extern char appraiseSit (void);

/**
 *  \brief Address a customer operation.
 *
 *  The entrepreneur goes to the counter to attend a customer.
 *
 *  \return the identification of the customer
 */

extern unsigned int addressACustomer (void);

/**
 *  \brief Say goodbye to customer operation.
 *
 *  The entrepreneur completes the transaction. The customer which was serviced should be waken up.
 *
 *  \param custId identification of the customer
 */

extern void sayGoodByeToCustomer (unsigned int custId);

/**
 *  \brief Customers in the shop operation.
 *
 *  The entrepreneur checks if there are any customers in the shop.
 *
 *  \return -c true, if there are any customers in the shop or about to enter the shop
 *  \return -c false, otherwise
 */

extern bool customersInTheShop (void);

/**
 *  \brief Close the door operation.
 *
 *  The entrepreneur closes the door to prevent further customers to come in.
 */

extern void closeTheDoor (void);

/**
 *  \brief Prepare to leave operation.
 *
 *  The entrepreneur closes the shop.
 */

extern void prepareToLeave (void);

/**
 *  \brief Go to the workshop operation.
 *
 *  The entrepreneur collects a batch of products from the storeroom at the workshop.
 */

extern void goToWorkShop (void);

/**
 *  \brief Visit suppliers operation.
 *
 *  The entrepreneur goes shopping for prime materials and delivers them to the workshop.
 */

extern void visitSuppliers (void);

/**
 *  \brief Return to shop.
 *
 *  The entrepreneur goes back to the shop.
 */

extern void returnToShop (void);

/**
 *  \brief End of operations for the entrepreneur.
 *
 *  Checking the end of the life cycle of the entrepreneur.
 *
 *  The entrepreneur stops if all prime materials have been converted into products and if all products have been
 *  sold and there are no requests of service pending and the shop is empty.
 *
 *  \return -c true, if the life cycle of the entrepreneur has come to an end
 *  \return -c false, otherwise
 */

extern bool endOperEntrep (void);

#endif /* SEMSHAREDMEMENTRP_H_ */
