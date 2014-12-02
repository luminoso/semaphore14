/**
 *  \file logging.c (implementation file)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "probConst.h"
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

void createLog (char nFic[])
{
  FILE *fic;                                                                                      /* file descriptor */
  char *dName = "log",                                                                      /* default log file name */
       *fName;                                                                                      /* log file name */
  unsigned int i;                                                                               /* counting variable */

  if ((nFic == NULL) || (strlen (nFic) == 0))
     fName = dName;
     else fName = nFic;
  if ((fic = fopen (fName, "w")) == NULL)
     { perror ("error on the creation of log file");
       exit (EXIT_FAILURE);
     }

  /* title line + blank line */

  fprintf (fic, "%21cAveiro Handicraft SARL - Description of the internal state\n\n", ' ');

  /* first line of field description */

  fprintf (fic, "ENTREPRE ");
  for (i = 0; i < N; i++)
    fprintf (fic, " CUST_%u ", i);
  fprintf (fic, " ");
  for (i = 0; i < M; i++)
    fprintf (fic, " CRAFT_%u", i);
  fprintf (fic, "%10cSHOP%8c", ' ', ' ');
  fprintf (fic, "%9cWORKSHOP\n", ' ');

  /* second line of field description */

  fprintf (fic, "  Stat   ");
  for (i = 0; i < N; i++)
    fprintf (fic, "Stat BP ");
  fprintf (fic, "  ");
  for (i = 0; i < M; i++)
    fprintf (fic, "Stat PP ");
  fprintf (fic, " Stat NCI NPI PCR PMR  ");
  fprintf (fic, "APMI NPI NSPM TAPM TNP\n");

  if (fclose (fic) == EOF)
     { perror ("error on closing of log file");
       exit (EXIT_FAILURE);
     }
}

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

void saveState (char nFic[], FULL_STAT *p_fSt)
{
  FILE *fic;                                                                                      /* file descriptor */
  char *dName = "log",                                                                      /* default log file name */
       *fName;                                                                                      /* log file name */
  unsigned int i;                                                                               /* counting variable */

  if ((nFic == NULL) || (strlen (nFic) == 0))
     fName = dName;
     else fName = nFic;
  if ((fic = fopen (fName, "a")) == NULL)
     { perror ("error on opening for appending of log file");
       exit (EXIT_FAILURE);
     }

  /* present full state description */

  switch (p_fSt->st.entrepStat)
  { case OPENING_THE_SHOP:               fprintf (fic, "  OPTS   ");
                                         break;
    case WAITING_FOR_NEXT_TASK:          fprintf (fic, "  WFNT   ");
                                         break;
    case ATTENDING_A_CUSTOMER:           fprintf (fic, "  ATAC   ");
                                         break;
    case CLOSING_THE_SHOP:               fprintf (fic, "  CLTS   ");
                                         break;
    case COLLECTING_A_BATCH_OF_PRODUCTS: fprintf (fic, "  CBOP   ");
                                         break;
    case DELIVERING_PRIME_MATERIALS:     fprintf (fic, "  DLPM   ");
                                         break;
    default:                             fprintf (fic, "  ****   ");
  }
  for (i = 0; i < N; i++)
  { switch (p_fSt->st.custStat[i].stat)
    { case CARRYING_OUT_DAILY_CHORES:   fprintf (fic, "CODC ");
                                        break;
      case CHECKING_SHOP_DOOR_OPEN:     fprintf (fic, "CSDO ");
                                        break;
      case APPRAISING_OFFER_IN_DISPLAY: fprintf (fic, "AOID ");
                                        break;
      case BUYING_SOME_GOODS:           fprintf (fic, "BYSG ");
                                        break;
      default:                          fprintf (fic, "**** ");
    }
    fprintf (fic, "%2u ", p_fSt->st.custStat[i].boughtPieces);
  }
  fprintf (fic, "  ");
  for (i = 0; i < M; i++)
  { switch (p_fSt->st.craftStat[i].stat)
    { case FETCHING_PRIME_MATERIALS:    fprintf (fic, "FTPM ");
                                        break;
      case PRODUCING_A_NEW_PIECE:       fprintf (fic, "PANP ");
                                        break;
      case STORING_IT_FOR_TRANSFER:     fprintf (fic, "SIFT ");
                                        break;
      case CONTACTING_THE_ENTREPRENEUR: fprintf (fic, "CTTE ");
                                        break;
      default:                          fprintf (fic, "**** ");
    }
    fprintf (fic, "%2u ", p_fSt->st.craftStat[i].prodPieces);
  }
  fprintf (fic, " ");
  switch (p_fSt->shop.stat)
  { case SOPEN:    fprintf (fic, "SPOP ");
                   break;
    case SDCLOSED: fprintf (fic, "SDCL ");
                   break;
    case SCLOSED:  fprintf (fic, "SPCL ");
                   break;
    default:       fprintf (fic, "**** ");
  }
  fprintf (fic, "%3u %3u ", p_fSt->shop.nCustIn, p_fSt->shop.nProdIn);
  if (p_fSt->shop.prodTransfer)
     fprintf (fic, " %c  ", 'T');
     else fprintf (fic, " %c  ", 'F');
  if (p_fSt->shop.primeMatReq)
     fprintf (fic, " %c   ", 'T');
     else fprintf (fic, " %c   ", 'F');
  fprintf (fic, "%3u  %3u %3u  %3u  %3u\n", p_fSt->workShop.nPMatIn, p_fSt->workShop.nProdIn,
                                            p_fSt->workShop.NSPMat, p_fSt->workShop.NTPMat, p_fSt->workShop.NTProd);

  if (fclose (fic) == EOF)
     { perror ("error on closing of log file");
       exit (EXIT_FAILURE);
     }
}
