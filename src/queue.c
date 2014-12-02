/**
 *  \file queue.c (implementation file)
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

#include <stdio.h>
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

void queueInit (QUEUE *p_q)
{ int i;                                                                                        /* counting variable */

  if (p_q == NULL) return;
  p_q->ii = p_q->ri = 0;
  p_q->full = false;
  for (i = 0; i < N; i++)
    p_q->mem[i] = EMPTYPOS;

}

/**
 *  \brief Insertion of a value into the queue.
 *
 *         The function fails if a null pointer is passed as a parameter or the queue is full.
 *         Nothing is stored if the function fails.
 *
 *  \param p_q pointer to the location where the queue is stored
 *  \param val value to be stored
 */

void queueIn (QUEUE *p_q, unsigned int val)
{
  if ((p_q == NULL) || (p_q->full)) return;
  p_q->mem[p_q->ii] = val;
  p_q->ii = (p_q->ii + 1) % N;
  p_q->full = (p_q->ii == p_q->ri);
}

/**
 *  \brief Retrieval of a value from the queue.
 *
 *         The function fails if a null pointer is passed as a parameter or the queue is empty.
 *         Nothing is retrieved if the function fails.
 *
 *  \param p_q pointer to the location where the queue is stored
 *  \param p_val pointer to the location where the retrieved value is to be stored
 */

void queueOut (QUEUE *p_q, unsigned int *p_val)
{
  if ((p_q == NULL) || (p_val == NULL) || (!(p_q->full) && (p_q->ii == p_q->ri))) return;
  *p_val = p_q->mem[p_q->ri];
  p_q->ri = (p_q->ri + 1) % N;
  p_q->full = false;
}

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

int queuePeek (QUEUE *p_q, unsigned int pos)
{
  if ((p_q == NULL) || (pos >= N) || (!(p_q->full) && (p_q->ii == p_q->ri))) return EMPTYPOS;
  if ((p_q->ii > p_q->ri) && (p_q->ii > (p_q->ri + pos)))
     return (int) p_q->mem[p_q->ri + pos];
     else if (p_q->ii <= p_q->ri)
             { if ((((p_q->ri + pos) % N) >= p_q->ri) || (((p_q->ri + pos) % N) < p_q->ii))
                  return (int) p_q->mem[(p_q->ri + pos) % N];
             }
  return EMPTYPOS;
}

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

bool queueFull (QUEUE *p_q)
{
  if (p_q == NULL) return false;
  return p_q->full;
}

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

bool queueEmpty (QUEUE *p_q)
{
  if (p_q == NULL) return false;
  return !(p_q->full) && (p_q->ii == p_q->ri);
}
