/**
 *  \file semSharedMemCraft.c (implementation file file)
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


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>

#include "probConst.h"
#include "probDataStruct.h"
#include "sharedDataSync.h"
#include "queue.h"
#include "logging.h"
#include "semaphore.h"
#include "sharedMemory.h"

/** \brief logging file name */
static char *nFic;

/** \brief shared memory block access identifier */
static int shmid;

/** \brief semaphore set access identifier */
static int semgid;

/** \brief pointer to shared memory region */
static SHARED_DATA *sh;

/** \brief collect materials operation */
static bool collectMaterials (unsigned int craftId);

/** \brief prime materials needed operation */
static void primeMaterialsNeeded (unsigned int craftId);

/** \brief back to work operation */
static void backToWork (unsigned int craftId);

/** \brief prepare to produce operation */
static void prepareToProduce (unsigned int craftId);

/** \brief go to store operation */
static unsigned int goToStore (unsigned int craftId);

/** \brief batch ready for transfer operation */
static void batchReadyForTransfer (unsigned int craftId);

/** \brief end of operations craftsman operation */
static bool endOperCraftsman (unsigned int craftId);

/** \brief shaping it up [internal] operation */
static void shapingItUp (void);

/**
 *  \brief Main program.
 *
 *  Its role is to generate the life cycle of one of intervening entities in the problem: the craftsman.
 */

int main (int argc, char *argv[])
{
  int key;                                                           /*access key to shared memory and semaphore set */
  char *tinp;                                                                      /* numerical parameters test flag */
  unsigned int m;                                                                         /* craftman identification */

  /* validation of comand line parameters */

  if (argc != 5)
     { freopen ("error_GCF", "a", stderr);
       fprintf (stderr, "Number of parameters is incorrect!\n");
       exit (EXIT_FAILURE);
     }
     else freopen (argv[4], "w", stderr);
  m = (unsigned int) strtol (argv[1], &tinp, 0);
  if ((*tinp != '\0') || (m >= M))
     { fprintf (stderr, "Craftman process id is invalid!\n");
       exit (EXIT_FAILURE);
     }
  nFic = argv[2];
  key = (unsigned int) strtol (argv[3], &tinp, 0);
  if (*tinp != '\0')
     { fprintf (stderr, "Error on the access key communication!\n");
       exit (EXIT_FAILURE);
     }

  /* connection to the semaphore set and the shared memory region and mapping the shared region on the process address
     space */

  if ((semgid = semConnect (key)) == -1)
     { perror ("error on connecting to the semaphore set");
       exit (EXIT_FAILURE);
     }
  if ((shmid = shmemConnect (key)) == -1)
     { perror ("error on connecting to the shared memory region");
       exit (EXIT_FAILURE);
     }
  if (shmemAttach (shmid, (void **) &sh) == -1)
     { perror ("error on mapping the shared region on the process address space");
       exit (EXIT_FAILURE);
     }

  /* simulation of the life cycle of the craftsman */

  unsigned int np;                                                                    /* number of products in store */
  bool alert;                                                               /* low level of prime materials in store */

  while (!endOperCraftsman (m))
  { alert = collectMaterials (m);        /* the craftsman gets the prime materials he needs to manufacture a product */
    if (alert)
       { primeMaterialsNeeded (m);                     /* the craftsman phones the entrepreneur to let her know the
                                                                              workshop requires more prime materials */
         backToWork (m);                                              /* the craftsman returns to his regular duties */
       }
    prepareToProduce (m);           /* the craftsman sits down and prepares things for the production of a new piece */
    shapingItUp ();                                                            /* the craftsman works on a new piece */
    np = goToStore (m);                                                 /* the craftsman stores the finished product */
    if (np >= MAX)                                                    /* the craftsman checks if it is transfer time */
       batchReadyForTransfer (m);                /* the craftsman phones the entrepreneur to let her know she should
                                                                            come and collect a new batch of products */
    backToWork (m);                                                   /* the craftsman returns to his regular duties */
  }

  /* unmapping the shared region off the process address space */

  if (shmemDettach (sh) == -1)
     { perror ("error on unmapping the shared region off the process address space");
       exit (EXIT_FAILURE);
     }

  exit (EXIT_SUCCESS);
}

/**
 *  Collect materials operation.
 *  The craftsman gets the prime materials he needs to manufacture a product.
 *
 *  input parameter:
 *    craftId --- identification of the craftsman
 *
 *  returned value:
 *    TRUE, if it is necessary to phone the entrepreneur to let her know the workshop requires more prime materials
 *    FALSE, otherwise
 */

static bool collectMaterials(unsigned int craftId) {
    if (semDown(semgid, sh->access) == -1) /* enter critical region */ {
        perror("error on executing the down operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    bool materialsRequired;

    while (!sh->fSt.workShop.nPMatIn) {
        sh->nCraftsmenBlk++;

        if (semUp(semgid, sh->access) == -1) /* exit critical region */ {
            perror("error on executing the up operation for semaphore access");
            exit(EXIT_FAILURE);
        }

        /* insert your code here */
        if (semDown(semgid, sh->waitForMaterials) == -1) {
            perror("collectMaterials() error during semDown waitformaterials");
            exit(EXIT_FAILURE);
        }

        if (semDown(semgid, sh->access) == -1) /* enter critical region */ {
            perror("error on executing the down operation for semaphore access");
            exit(EXIT_FAILURE);
        }
    }

    /* insert your code here */
    sh->fSt.workShop.nPMatIn--;
    saveState(nFic, &(sh->fSt));

    materialsRequired = (sh->fSt.workShop.NSPMat <= NP) && (sh->fSt.workShop.nPMatIn <= PMIN);

    if (semUp(semgid, sh->access) == -1) /* exit critical region */ {
        perror("error on executing the up operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    return materialsRequired;
}

/**
 *  \brief Prime materials needed operation.
 *
 *  The craftsman phones the entrepreneur to let her know the workshop requires more prime materials.
 *
 *  \param craftId identification of the craftsman
 */

static void primeMaterialsNeeded (unsigned int craftId)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  sh->fSt.st.craftStat[craftId].stat = CONTACTING_THE_ENTREPRENEUR;
  sh->fSt.shop.primeMatReq = true;
  
  if(semUp(semgid,sh->proceed) == -1){
      perror("primeMaterialsNeeded() error during semUp() for proceed");
      exit (EXIT_FAILURE);
  }
  
  saveState(nFic,&(sh->fSt));

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

/**
 *  \brief Back to work operation.
 *
 *  The craftsman returns to his regular duties.
 *
 *  \param craftId identification of the craftsman
 */

static void backToWork (unsigned int craftId)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  sh->fSt.st.craftStat[craftId].stat = FETCHING_PRIME_MATERIALS;
  saveState(nFic,&(sh->fSt));

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

/**
 *  \brief Prepare to produce operation.
 *
 *  The craftsman sits down and prepares things for the production of a new piece.
 *
 *  \param craftId identification of the craftsman
 */

static void prepareToProduce (unsigned int craftId)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  sh->fSt.st.craftStat[craftId].stat = PRODUCING_A_NEW_PIECE;
  saveState(nFic,&(sh->fSt));

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

/**
 *  \brief Go to store operation.
 *
 *  The craftsman stores the finished product.
 *
 *  \param craftId identification of the craftsman
 *
 *  \return number of products presently stored in the storeroom
 */

static unsigned int goToStore (unsigned int craftId)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  int nProdIn;
  sh->fSt.st.craftStat[craftId].stat = STORING_IT_FOR_TRANSFER;
  sh->fSt.st.craftStat[craftId].prodPieces++;
  sh->fSt.workShop.nProdIn++;
  sh->fSt.workShop.NTProd++;
  saveState(nFic,&(sh->fSt));
  nProdIn = sh->fSt.workShop.nProdIn;

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  return nProdIn;
}

/**
 *  \brief Batch ready for transfer operation.
 *
 *  The craftsman phones the entrepreneur to let her know she should collect a batch of goods.
 *
 *  \param craftId identification of the craftsman
 */

static void batchReadyForTransfer (unsigned int craftId)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  sh->fSt.st.craftStat[craftId].stat = CONTACTING_THE_ENTREPRENEUR;
  sh->fSt.shop.prodTransfer = true;
  
  if(semUp(semgid,sh->proceed) == -1){
      perror("batchReadyForTransfer() error while semUp sh->proceed");
      exit(EXIT_FAILURE);
  }

  saveState(nFic,&(sh->fSt));
  
  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

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

static bool endOperCraftsman (unsigned int craftId)
{
  bool stat;                                                                                     /* craftsman status */
  unsigned int nOpCraft,                                                      /* number of craftsmen still operative */
               i;                                                                               /* counting variable */

  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  stat = (sh->fSt.workShop.NSPMat == NP);               /* all the delivers of prime materials have been carried out */
  if (stat)
     { nOpCraft = 0;                                              /* find out how many customers are still operative */
       for (i = 0; i < M; i++)
         if (sh->fSt.st.craftStat[i].readyToWork) nOpCraft += 1;
       stat = (sh->fSt.workShop.nPMatIn < nOpCraft*PP);             /* the amount of prime materials still remaining
                                               is less than the number of craftsmen still operative times the amount
                                                                     of prime materials necessary to produce a piece */
       if (stat)
          sh->fSt.st.craftStat[craftId].readyToWork = false;  /* the craftsman is signaled non operative from now on */
       if (stat && (nOpCraft == 1))                                   /* check if the last craftsman is about to die */
          { sh->fSt.shop.prodTransfer = true;                    /* signal a batch of products is ready for transfer */
            if (semUp (semgid, sh->proceed) == -1)                                     /* and alert the entrepreneur */
               { perror ("error on executing the up operation for semaphore proceed");
                 exit (EXIT_FAILURE);
               }
            saveState (nFic, &(sh->fSt));                                      /* save present state in the log file */
          }
     }

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  stat = true;                               /*         <---            remove this instruction for normal operation */
  return stat;
}

/**
 *  \brief Shaping it up operation.
 *
 *  The craftsman manufactures a new piece for a random generated time interval.
 */

static void shapingItUp (void)
{
  usleep((unsigned int) floor (30.0 * random () / RAND_MAX + 1.5));
}
