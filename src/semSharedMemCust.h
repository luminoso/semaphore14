/**
 *  \file semSharedMemCust.h (interface file)
 *
 *  \brief Problem name: Aveiro Handicraft SARL.
 *
 *  \brief Concept: Pedro Mariano.
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  Definition of the operations carried out by the customers:
 *     \li goShopping
 *     \li isDoorOpen
 *     \li tryAgainLater
 *     \li enterShop
 *     \li perusingAround
 *     \li iWantThis
 *     \li exitShop
 *     \li endOperCustomer.
 *
 *  \author António Rui Borges - October 2014
 */

#ifndef SEMSHAREDMEMCUST_H_
#define SEMSHAREDMEMCUST_H_

#include <stdbool.h>

/**
 *  \brief Go shopping operation.
 *
 *  The customer decides to visit the handicraft shop.
 *
 *  \param custId identification of the customer
 */

extern void goShopping (unsigned int custId);

/**
 *  \brief Is door open operation.
 *
 *  The customer checks if the shop door is open.
 *
 *  \param custId identification of the customer
 *
 *  \return -c true, if the shop door is open
 *  \return -c false, otherwise
 */

extern bool isDoorOpen (unsigned int custId);

/**
 *  \brief Try again later operation.
 *
 *  The customer goes back to perform his daily chores.
 *
 *  \param custId identification of the customer
 */

extern void tryAgainLater (unsigned int custId);

/**
 *  \brief   Enter the shop operation.
 *
 *  The customer enters the shop.
 *
 *  \param custId identification of the customer
 */

extern void enterShop (unsigned int custId);

/**
 *  \brief Perusing around operation.
 *
 *  The customer inspects the offer in display and eventually picks up some goods.
 *  He may randomly pick up 0, 1 or 2 products.
 *
 *  \param custId identification of the customer
 *
 *  \return number of goods to buy
 */

extern unsigned int perusingAround (unsigned int custId);

/**
 *  \brief I want this operation.
 *
 *  The customer queues by the counter to pay for the selected goods.
 *
 *  \param custId identification of the customer
 *  \param nGoods number of selected goods
 */

extern void iWantThis (unsigned int custId, unsigned int nGoods);

/**
 *  \brief Exit the shop operation.
 *
 *  The customer leaves the shop.
 *
 *  \param custId identification of the customer
 */

extern void exitShop (unsigned int custId);

/**
 *  \brief End of operations for the customer.
 *
 *  Checking the end of life cycle of the customer.
 *  The customer stops if all prime materials have been converted into products and if the amount of products still
 *  remaining to be sold is less than the number of customers still active times 2.
 *
 *  \param custId identification of the customer
 *
 *  \return -c true, if the life cycle of the customer has come to an end
 *  \return -c false, otherwise
 */

extern bool endOperCustomer (unsigned int custId);

#endif /* SEMSHAREDMEMCUST_H_ */
