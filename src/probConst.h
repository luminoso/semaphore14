/**
 *  \file probConst.h (interface file)
 *
 *  \brief Problem name: Aveiro Handicraft SARL.
 *
 *  \brief Concept: Pedro Mariano.
 *
 *  Problem simulation parameters.
 *
 *  \author António Rui Borges - October 2014
 */

#ifndef PROBCONST_H_
#define PROBCONST_H_

/* Generic parameters */

/** \brief number of customers */
#define  N           3
/** \brief number of craftsmen */
#define  M           3
/** \brief storeroom nominal capacity (in number of products) */
#define  MAX         4
/** \brief minimum acceptable level of prime materials in storage */
#define  PMIN        2
/** \brief amount of prime materials required to produce a piece */
#define  PP          1
/** \brief number of times prime materials are supplied */
#define  NP          4

/* Constants defining entrepreneur state */

/** \brief the entrepreneur is opening the shop */
#define  OPENING_THE_SHOP                0
/** \brief the entrepreneur is waiting for service requests */
#define  WAITING_FOR_NEXT_TASK           1
/** \brief the entrepreneur is attending a customer */
#define  ATTENDING_A_CUSTOMER            2
/** \brief the entrepreneur is closing the shop */
#define  CLOSING_THE_SHOP                3
/** \brief the entrepreneur is collecting finished products from the work shop */
#define  COLLECTING_A_BATCH_OF_PRODUCTS  4
/** \brief the entrepreneur goes shopping for prime materials and delivers them to the work shop */
#define  DELIVERING_PRIME_MATERIALS      5

/* Constants defining customers state */

/** \brief the customer is living his normal life */
#define  CARRYING_OUT_DAILY_CHORES       0
/** \brief the customer is checking whether he can enter the shop */
#define  CHECKING_SHOP_DOOR_OPEN         1
/** \brief the customer is inspecting the products being offered */
#define  APPRAISING_OFFER_IN_DISPLAY     2
/** \brief the customer is buying some products */
#define  BUYING_SOME_GOODS               3

/* Constants defining craftsmen state */

/** \brief the craftsman is fetching the prime materials he needs to produce a new piece */
#define  FETCHING_PRIME_MATERIALS        0
/** \brief the craftsman is producing a new piece */
#define  PRODUCING_A_NEW_PIECE           1
/** \brief the craftsman is placing the produced piece in the storeroom */
#define  STORING_IT_FOR_TRANSFER         2
/** \brief the craftsman is contacting the entrepreneur either to collect the produced pieces or to deliver prime materials */
#define  CONTACTING_THE_ENTREPRENEUR     3

#endif /* PROBCONST_H_ */
