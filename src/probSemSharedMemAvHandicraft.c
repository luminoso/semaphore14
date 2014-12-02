/**
 *  \file probSemSharedMemAvHandicraft.c (implementation file)
 *
 *  \brief Problem name: Aveiro Handicraft SARL.
 *
 *  \brief Concept: Pedro Mariano.
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  Generator process of the intervening entities.
 *
 *  Upon execution, one parameter is requested:
 *    \li name of the logging file.
 *
 *  \author António Rui Borges - October 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <math.h>

#include "probConst.h"
#include "probDataStruct.h"
#include "queue.h"
#include "logging.h"
#include "sharedDataSync.h"
#include "semaphore.h"
#include "sharedMemory.h"

/** \brief name of entrepreneur process */
#define   ENTREPRENEUR   "./entrepreneur"

/** \brief name of customer process */
#define   CUSTOMER       "./customer"

/** \brief name of craftsman process */
#define   CRAFTSMAN      "./craftsman"

/**
 *  \brief Main program.
 *
 *  Its role is starting the simulation by generating the intervening entities processes (entrepreneur, customers and craftsmen)
 *  and waiting for their termination.
 */

int main (int argc, char *argv[])
{
  char nFic[21];                                                                              /*name of logging file */
  char nFicErr[] = "error____";                                                          /* base name of error files */
  FILE *fic;                                                                                      /* file descriptor */
  int shmid,                                                                      /* shared memory access identifier */
      semgid;                                                                     /* semaphore set access identifier */
  int t;                                                                               /* keyboard reading test flag */
  char opt;                                                                                                /* answer */
  unsigned int i, n;                                                                           /* counting variables */
  SHARED_DATA *sh;                                                                /* pointer to shared memory region */
  int pidE,                                                                       /* entrepreneur process identifier */
      pidCT[N],                                                                    /* customer processes identifiers */
      pidCF[M];                                                                   /* craftsman processes identifiers */
  int key;                                                           /*access key to shared memory and semaphore set */
  char num[2][12];                                                     /* numeric value conversion (up to 10 digits) */
  int status,                                                                                    /* execution status */
      info;                                                                                               /* info id */
  bool term;                                                                             /* process termination flag */

  /* getting log file name */

  do
  { do
    { printf ("\nLog file name? ");
      t = scanf ("%20[^\n]", nFic);
      scanf ("%*[^\n]");
      scanf ("%*c");
    } while (t == 0);
    fic = fopen (nFic, "r");
    if (fic != NULL)
       { fclose (fic);
         printf ("There is already a file with this name! ");
         do
         { printf ("Overwrite? ");
           scanf ("%c", &opt);
           if (opt != '\n')
              { scanf ("%*[^\n]");
                scanf ("%*c");
              }
         } while ((opt == '\n') || ((opt != 'Y') && (opt != 'y') &&
                                    (opt != 'N') && (opt != 'n')));
         if ((opt == 'Y') || (opt == 'y')) break;
       }
  } while (fic != NULL);

  /* composing command line */

  sprintf (num[0], "%s", "0");
  if ((key = ftok (".", 'a')) == -1)
     { perror ("error on generating the key");
       exit (EXIT_FAILURE);
     }
  sprintf (num[1], "%d", key);

  /* creating and initializing the shared memory region and the log file */

  if ((shmid = shmemCreate (key, sizeof (SHARED_DATA))) == -1)
     { perror ("error on creating the shared memory region");
       exit (EXIT_FAILURE);
     }
  if (shmemAttach (shmid, (void **) &sh) == -1)
     { perror ("error on mapping the shared region on the process address space");
       exit (EXIT_FAILURE);
     }

  srandom ((unsigned int) getpid ());                                                 /* initialize random generator */

    /* initialize the amounts of prime materials to be supplied each time */

  unsigned int total;                                                    /* total amount of prime materials supplied */

  for (total = 0, i = 0; i < NP; i++)
  { sh->fSt.primeMaterials[i] = (unsigned int) floor (10.0*PP*random ()/RAND_MAX+PP+0.5); /* set the amount supplied
                                                                                                           each time */
    total += sh->fSt.primeMaterials[i];
  }
  if (sh->fSt.primeMaterials[NP-1] < 2*PP*M)
     { total += 2*PP*M - sh->fSt.primeMaterials[NP-1];            /* make the end of simulation easier to be reached */
       sh->fSt.primeMaterials[NP-1] = 2*PP*M;
     }
  if (total % PP != 0) sh->fSt.primeMaterials[NP-1] += PP - total % PP;

    /* initialize problem internal status */

  sh->fSt.st.entrepStat = OPENING_THE_SHOP;                                  /* the entrepreneur is opening the shop */
  for (i = 0; i < N; i++)
  { sh->fSt.st.custStat[i].stat = CARRYING_OUT_DAILY_CHORES;                      /* the customer is living his life */
    sh->fSt.st.custStat[i].boughtPieces = 0;                                          /* no goods were bought so far */
    sh->fSt.st.custStat[i].readyToWork = true;                      /* the customer is operative - availability flag
                                                                                          required by the simulation */
  }
  for (i = 0; i < M; i++)
  { sh->fSt.st.craftStat[i].stat = FETCHING_PRIME_MATERIALS;        /* the craftsman is fetching the prime materials
                                                                                     he needs to produce a new piece */
    sh->fSt.st.craftStat[i].prodPieces = 0;                                        /* no pieces were produced so far */
    sh->fSt.st.craftStat[i].readyToWork = true;                     /* the customer is operative - availability flag
                                                                                          required by the simulation */
  }
  sh->fSt.shop.stat = SCLOSED;                                                                 /* the shop is closed */
  sh->fSt.shop.nCustIn = 0;                                            /* no customers are presently inside the shop */
  sh->fSt.shop.nProdIn = 0;                                                      /* the shop has no products to sell */
  sh->fSt.shop.prodTransfer = false;                 /* no craftsman has phoned yet to request the transfer of a new
                                                                                                   batch of products */
  sh->fSt.shop.primeMatReq = false;    /* no craftsman has phoned yet asking for the deliver of more prime materials */
  queueInit (&(sh->fSt.shop.queue));                                                /* waiting queue is set to empty */
  sh->fSt.workShop.nPMatIn = sh->fSt.primeMaterials[0];      /* first deliver of prime materials is presently stored
                                                                                                 inside the workshop */
  sh->fSt.workShop.nProdIn = 0;                                                  /* the storeroom is presently empty */
  sh->fSt.workShop.NSPMat = 1;                             /* number of delivers of prime materials is presently one */
  sh->fSt.workShop.NTPMat = sh->fSt.primeMaterials[0];         /* initial storage of prime materials in the workshop */
  sh->fSt.workShop.NTProd = 0;                                                 /* no products have been produced yet */
  sh->nCraftsmenBlk = 0;                                  /* no craftsman threads is waiting for prime materials yet */

    /* initialize problem internal status */

  createLog (nFic);                                                                             /* log file creation */
  saveState (nFic, &(sh->fSt));                                                               /* store initial state */

    /* initialize semaphore ids */

  sh->access = ACCESS;                                                              /* mutual exclusion semaphore id */
  sh->proceed = PROCEED;                                             /* entrepreneur appraise situation semaphore id */
  sh->waitForMaterials = WAITFORMATERIALS;                     /* craftsmen waiting for prime materials semaphore id */
  for (i = 0; i < N; i++)
    sh->waitForService[i] = B_WAITFORSERVICE + i;                      /*customers waiting for service semaphores id */

  /* creating and initializing the semaphore set */

  if ((semgid = semCreate (key, SEM_NU)) == -1)
     { perror ("error on creating the semaphore set");
       exit (EXIT_FAILURE);
     }
  if (semUp (semgid, sh->access) == -1)                                        /* enabling access to critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* generation of intervening entities processes */

  if ((pidE = fork ()) < 0)                                                                  /* entrepreneur process */
     { perror ("error on the fork operation for the entrepreneur");
       exit (EXIT_FAILURE);
     }
  strcpy (nFicErr + 6, "ET");
  if (pidE == 0)
     if (execl (ENTREPRENEUR, ENTREPRENEUR, nFic, num[1], nFicErr, NULL) < 0)
          { perror ("error on the generation of the entrepreneur process");
            exit (EXIT_FAILURE);
          }
  strcpy (nFicErr + 6, "CT");
  for (i = 0; i < N; i++)                                                                     /* customers processes */
  { if ((pidCT[i] = fork ()) < 0)
       { perror ("error on the fork operation for the customer");
         exit (EXIT_FAILURE);
       }
    num[0][0] = '0' + i;
    nFicErr[8] = '0' + i;
    if (pidCT[i] == 0)
       if (execl (CUSTOMER, CUSTOMER, num[0], nFic, num[1], nFicErr, NULL) < 0)
          { perror ("error on the generation of the customer process");
            exit (EXIT_FAILURE);
          }
  }
  strcpy (nFicErr + 6, "CF");
  for (i = 0; i < M; i++)                                                                     /* craftsmen processes */
  { if ((pidCF[i] = fork ()) < 0)
       { perror ("error on the fork operation for the craftsman");
         exit (EXIT_FAILURE);
       }
    num[0][0] = '0' + i;
    nFicErr[8] = '0' + i;
    if (pidCF[i] == 0)
       if (execl (CRAFTSMAN, CRAFTSMAN, num[0], nFic, num[1], nFicErr, NULL) < 0)
          { perror ("error on the generation of the craftsman process");
            exit (EXIT_FAILURE);
          }
  }

  /* signaling start of operations */

  if (semSignal (semgid) == -1)
     { perror ("error on signaling start of operations");
       exit (EXIT_FAILURE);
     }

  /* waiting for the termination of the intervening entities processes */

  printf ("\nFinal report\n");
  n = 0;
  do
  { info = wait (&status);
    term = false;
    for (i = 0; i < N+M+1; i++)
      if ((i == 0) && (info == pidE))
         { term = true;
           break;
         }
	 else if ((i > 0) && (i <= N) && (info == pidCT[i-1]))
                 { term = true;
                   break;
                 }
		 else if ((i > N) && (i <= N+M) && (info == pidCF[i-N-1]))
                         { term = true;
                           break;
                         }
    if (!term)
       { perror ("error on waiting for an intervening process");
         exit (EXIT_FAILURE);
       }
    if (i == 0)
       printf ("the entrepreneur process has terminated: ");
       else if (i <= N)
	       printf ("the customer process, with id %u, has terminated: ", i-1);
               else printf ("the craftsman process, with id %u, has terminated: ", i-N-1);
    if (WIFEXITED (status))
       printf ("its status was %d\n", WEXITSTATUS (status));
    n += 1;
  } while (n < N+M+1);

  /* destruction of semaphore set and shared region */

  if (semDestroy (semgid) == -1)
     { perror ("error on destructing the semaphore set");
       exit (EXIT_FAILURE);
     }
  if (shmemDettach (sh) == -1)
     { perror ("error on unmapping the shared region off the process address space");
       exit (EXIT_FAILURE);
     }
  if (shmemDestroy (shmid) == -1)
     { perror ("error on destructing the shared region");
       exit (EXIT_FAILURE);
     }

  return EXIT_SUCCESS;
}
