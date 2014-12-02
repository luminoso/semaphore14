/**
 *  \file logging.h (interface file)
 *
 *  Problem name: Aveiro Handicraft SARL
 *
 *  Concept: Pedro Mariano
 *
 *  \brief Logging the internal state of the problem into a file.
 *
 *  Defined operations:
 *     \li file initialization
 *     \li writing the present state as a single line at the end of the file.
 *
 *  \author António Rui Borges - October 2014
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include "probDataStruct.h"

/**
 *  \brief File initialization.
 *
 *  The function creates the logging file and writes its header.
 *  If <tt>nFic</tt> is a null pointer or a null string, the file is created under a predefined name <em>log</em>.
 *
 *  The header consists of
 *       \li a line title
 *       \li a blank line
 *       \li a double line describing the meaning of the different fields of the state line.
 *
 *  \param nFic name of the logging file
 */

extern void createLog (char nFic[]);

/**
 *  \brief Writing the present full state as a single line at the end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are appended to a file under the predefined
 *  name <em>log</em>.
 *
 *  The following layout is obeyed for the full state in a single line
 *    \li entrepreneur state
 *    \li customers state (n = 0,...,N-1)
 *    \li craftsmen state (m = 0,..., M-1)
 *    \li shop state
 *    \li work shop state.
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

extern void saveState (char nFic[], FULL_STAT *p_fSt);

#endif /* LOGGING_H_ */
