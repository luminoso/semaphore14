/**
 *  \file queue.h (interface file)
 *
 *  \brief Problem name: Aveiro Handicraft SARL.
 *
 *  \brief Concept: Pedro Mariano.
 *
 *  Queue management.
 *
 *  The following operations are defined:
 *     \li initialization
 *     \li insertion of a value
 *     \li retrieval of a value
 *     \li test for queue full
 *     \li test for queue empty.
 *
 *  \author António Rui Borges - October 2014
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>

#include "probConst.h"
#include "probDataStruct.h"

/**
 *  \brief Queue initialization.
 *
 *         Queue will be empty after it.
 *         The function fails if a null pointer is passed as a parameter.
 *
 *  \param p_q pointer to the location where the queue is stored
 */

extern void queueInit (QUEUE *p_q);

/**
 *  \brief Insertion of a value into the queue.
 *
 *         The function fails if a null pointer is passed as a parameter or the queue is full.
 *         Nothing is stored if the function fails.
 *
 *  \param p_q pointer to the location where the queue is stored
 *  \param val value to be stored
 */

extern void queueIn (QUEUE *p_q, unsigned int val);

/**
 *  \brief Retrieval of a value from the queue.
 *
 *         The function fails if a null pointer is passed as a parameter or the queue is empty.
 *         Nothing is retrieved if the function fails.
 *
 *  \param p_q pointer to the location where the queue is stored
 *  \param p_val pointer to the location where the retrieved value is to be stored
 */

extern void queueOut (QUEUE *p_q, unsigned int *p_val);

/**
 *  \brief Peek at a value from the queue.
 *
 *         The function fails if a null pointer is passed as a parameter or the queue is empty or
 *         position is out of range.
 *
 *  \param p_q pointer to the location where the queue is stored
 *  \param pos queue position to be peeked
 *
 *  \return  the peeked value, if there is one
 *  \return  \c EMPTYPOS, otherwise
 */

extern int queuePeek (QUEUE *p_q, unsigned int pos);

/**
 *  \brief Test for queue full.
 *
 *         The function fails if a null pointer is passed as a parameter.
 *
 *  \param p_q pointer to the location where the queue is stored
 *
 *  \return \c true, if the queue is full
 *  \return \c false, otherwise
 */

extern bool queueFull (QUEUE *p_q);

/**
 *  \brief Test for queue empty.
 *
 *         The function fails if a null pointer is passed as a parameter.
 *
 *  \param p_q pointer to the location where the queue is stored
 *
 *  \return \c true, if the queue is empty
 *  \return \c false, otherwise
 */

extern bool queueEmpty (QUEUE *p_q);

#endif /* QUEUE_H_ */
