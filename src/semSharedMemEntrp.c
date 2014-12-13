/**
 *  \file semSharedMemEntrp.c (implementation file)
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

/** \brief prepare to work operation */
static void prepareToWork (void);

/** \brief appraise situation operation */
static char appraiseSit (void);

/** \brief address a customer operation */
static unsigned int addressACustomer (void);

/** \brief appraise situation operation */
static void sayGoodByeToCustomer (unsigned int custId);

/** \brief customer in the shop operation */
static bool customersInTheShop (void);

/** \brief close the door operation */
static void closeTheDoor (void);

/** \brief prepare to leave operation */
static void prepareToLeave (void);

/** \brief go to workshop operation */
static void goToWorkShop (void);

/** \brief visit suppliers operation */
static void visitSuppliers (void);

/** \brief return to shop operation */
static void returnToShop (void);

/** \brief end of operations entrepreneur operation */
static bool endOperEntrep (void);

/** \brief service customer [internal] operation */
static void serviceCustomer (void);

/**
 *  \brief Main program.
 *
 *  Its role is to generate the life cycle of one of intervening entities in the problem: the entrepreneur.
 */

int main (int argc, char *argv[])
{
  int key;                                                           /*access key to shared memory and semaphore set */
  char *tinp;                                                                      /* numerical parameters test flag */

  /* validation of passed parameters */

  if (argc != 4)
     { freopen ("error_GET", "a", stderr);
       fprintf (stderr, "Number of parameters is incorrect!\n");
       exit (EXIT_FAILURE);
     }
     else freopen (argv[3], "w", stderr);
  nFic = argv[1];
  key = (unsigned int) strtol (argv[2], &tinp, 0);
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

  /* simulation of the life cicle of the entrepreneur */

  unsigned int c;                                                                                     /* customer id */
  char nextTask;                                                                           /* task to be carried out */
  bool busy;                         /* in-the-shop signaling flag: true, if the entrepreneur is working in the shop
                                                                                                    false, otherwise */

  while (!endOperEntrep ())
  { prepareToWork ();                        /* the entrepreneur opens the shop and gets ready to perform her duties */
    busy = true;                                                                                    /* set busy flag */
    while (busy)
    { nextTask = appraiseSit ();                                      /* the entrepreneur waits for service requests */
      if (nextTask == 'C')                             /* the entrepreneur checks if a customer is needing attention */
         { c = addressACustomer ();                     /* the entrepreneur goes to the counter to attend a customer */
           serviceCustomer ();                                             /* the entrepreneur services the customer */
           sayGoodByeToCustomer (c);                                   /* the entrepreneur completes the transaction */
         }
         else { busy = false;                                                                     /* reset busy flag */
                if ((nextTask == 'G') || (nextTask == 'P'))         /* the entrepreneur checks if some craftsman has
                                                                                                          phoned him */
                 { if (customersInTheShop ())                   /* checks if there are any customers inside the shop */
                      { closeTheDoor ();   /* the entrepreneur closes the door to prevent further customers to enter */
                        busy = true;                                                                /* set busy flag */
                      }
                 }
              }
    }
    prepareToLeave ();                                                           /* the entrepreneur closes the shop */
    if (nextTask == 'G')
       goToWorkShop ();                                                /* the entrepreneur collects a batch of goods */
       else if (nextTask == 'P')
             visitSuppliers ();                            /* the entrepreneur goes shopping for prime materials and
                                                                                       delivers them to the workshop */
    returnToShop ();                                                       /* the entrepreneur goes back to the shop */
  }

  /* unmapping the shared region off the process address space */

  if (shmemDettach (sh) == -1)
     { perror ("error on unmapping the shared region off the process address space");
       exit (EXIT_FAILURE);
     }

  exit (EXIT_SUCCESS);
}

/**
 *  \brief Prepare to work operation.
 *
 *  The entrepreneur opens the shop and gets ready to perform her duties.
 */

static void prepareToWork (void)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */

    sh->fSt.st.entrepStat = WAITING_FOR_NEXT_TASK; // change entrepreneur state
    sh->fSt.shop.stat = SOPEN; // open the shop
    saveState(nFic, &(sh->fSt));

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

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

static char appraiseSit (void)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  // senta na cadeira aguardar pedidos de solicitacao - FAZER SLEEP SOBRE A VARIAVEL PROCEED
  // fazemos isso pondo-a a dormir
  
  // se fila de espera n tiver vazia return 'C';
  // se a flag correspondente da chamada do artesao return P
  // se a flag de recolha estiver verdadeira return G
  // se (copiar a funcao toda endOperEntrep) sair com 'E' xit
  // se n for nenhum destes voltas a dormir
  
  // estrutura repetitiva. em algumas situacoes acorda noutras fica cá
  // estrutura repetitiva quer dizer: while(true se n for E,G,C,P)
  
  /*
   * while(true)
   *  se puder decisao
   *   implica decisao e salta fora
   */

     if (semUp (semgid, sh->access) == -1)                                                   /* exit critical region */
       { perror ("error on executing the up operation for semaphore access");
         exit (EXIT_FAILURE);
       }

  /* insert your code here  <---- é aqui bloqueia. ta fora da regiao critica */

    if (semDown (semgid, sh->access) == -1)                                                /* enter critical region */
       { perror ("error on executing the down operation for semaphore access");
         exit (EXIT_FAILURE);
       }

  /* insert your code here */

  if (semUp (semgid, sh->access) == -1)                                                     /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  return 'E';
}

/**
 *  \brief Address a customer operation.
 *
 *  The entrepreneur goes to the counter to attend a customer.
 *
 *  \return the identification of the customer
 */

static unsigned int addressACustomer (void)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  // mudar o estado da dona da empresa passa de waitingnextask para attending a customer
  // ver se a queue esta vazia ou n : se estiver erro de consistencia
  // devolve a identificacao de um cliente tambem verificando se é valido, sneao inconsistencia
  // savestate no fim
  
  unsigned int customerIdx;
  
  sh->fSt.st.entrepStat = ATTENDING_A_CUSTOMER;
  if(queueEmpty(&(sh->fSt.shop.queue))){
      perror("addressACustomer() - there is no customers in the queue");
      exit(EXIT_FAILURE);
  }
  
  queueOut(&(sh->fSt.shop.queue),&customerIdx);
  
  if(customerIdx >= N){
      perror("addressACustomer() - customer ID is inconsistent");
      exit(EXIT_FAILURE);
  }
  
  saveState(nFic,&(sh->fSt));

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit(EXIT_FAILURE);
     }

  return customerIdx;
}

/**
 *  \brief Say goodbye to customer operation.
 *
 *  The entrepreneur completes the transaction. The customer which was serviced should be waken up.
 *
 *  \param custId identification of the customer
 */

static void sayGoodByeToCustomer (unsigned int custId)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  // up no waitforservice[custID]
  // mudar o estado
  // save state no fim
  
  sh->fSt.st.entrepStat = WAITING_FOR_NEXT_TASK;
  
  if(semUp(semgid,sh->waitForService[custId]) == -1){
      perror ("error on executing the down operation for semaphore access");
      exit (EXIT_FAILURE);
  }
  
  saveState(nFic,&sh->fSt);
  
  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

/**
 *  \brief Customers in the shop operation.
 *
 *  The entrepreneur checks if there are any customers in the shop.
 *
 *  \return -c true, if there are any customers in the shop or about to enter the shop
 *  \return -c false, otherwise
 */

static bool customersInTheShop (void)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  bool customersInside;
  customersInside = sh->fSt.shop.nCustIn != 0;

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  return customersInside;
}

/**
 *  \brief Close the door operation.
 *
 *  The entrepreneur closes the door to prevent further customers to come in.
 */

static void closeTheDoor (void)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  sh->fSt.shop.stat = SDCLOSED;
  saveState(nFic,&(sh->fSt));

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

/**
 *  \brief Prepare to leave operation.
 *
 *  The entrepreneur closes the shop.
 */

static void prepareToLeave (void)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  // SO MUDANCA DE ESTADO
  
  sh->fSt.shop.stat = SCLOSED;
  saveState (nFic, &(sh->fSt));

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

/**
 *  \brief Go to the workshop operation.
 *
 *  The entrepreneur collects a batch of products from the storeroom at the workshop.
 */

static void goToWorkShop (void)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  // mudar o estado
  // acordar o numero de artesaos que está em nCraftmemeBlk
  
  sh->fSt.st.entrepStat = COLLECTING_A_BATCH_OF_PRODUCTS;
  sh->fSt.shop.nProdIn += sh->fSt.workShop.nProdIn;
  sh->fSt.workShop.nProdIn = 0;
  sh->fSt.shop.prodTransfer = false;
  saveState(nFic,&(sh->fSt));  

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

/**
 *  \brief Visit suppliers operation.
 *
 *  The entrepreneur goes shopping for prime materials and delivers them to the workshop.
 */

static void visitSuppliers (void)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  unsigned int craftmenCounter;
  
  sh->fSt.st.entrepStat = DELIVERING_PRIME_MATERIALS;
  sh->fSt.shop.primeMatReq = false;
  
  if(sh->fSt.workShop.NSPMat <= NP){ // <= ou <
      sh->fSt.workShop.nPMatIn += sh->fSt.primeMaterials[sh->fSt.workShop.NSPMat];
      sh->fSt.workShop.NTPMat += sh->fSt.primeMaterials[sh->fSt.workShop.NSPMat++];
  }
  
  craftmenCounter = 0;
  
  while(sh->nCraftsmenBlk > 0){
      // fazer nCraftsmenBlk vezes up a um semaforo?
      if (semUp(semgid,sh->waitForMaterials) == -1){
          perror("visitSuppliers() error during semUP waiting for materials");
          exit(EXIT_FAILURE);
      }
      craftmenCounter++;
  }
  
  sh->nCraftsmenBlk = 0;
  saveState(nFic,&(sh->fSt));

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

/**
 *  \brief Return to shop.
 *
 *  The entrepreneur goes back to the shop.
 */

static void returnToShop (void)
{
  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  /* insert your code here */
  sh->fSt.st.entrepStat = OPENING_THE_SHOP;
  saveState(nFic,&(sh->fSt));

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }
}

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

static bool endOperEntrep (void)
{
  bool stat;                                                                                  /* entrepreneur status */

  if (semDown (semgid, sh->access) == -1)                                                   /* enter critical region */
     { perror ("error on executing the down operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  stat = (sh->fSt.shop.nCustIn == 0) &&                                          /* the shop has no customers in and */
         (sh->fSt.shop.nProdIn == 0) &&                                /* all products in display have been sold and */
         !sh->fSt.shop.primeMatReq &&                       /* no craftsman has phoned to request prime materials or */
         !sh->fSt.shop.prodTransfer &&                         /* to ask for a batch of products to be collected and */
         (sh->fSt.workShop.nProdIn == 0) &&   /* there are no finished products in the storeroom at the workshop and */
         (sh->fSt.workShop.nPMatIn == 0) &&               /* all prime materials at the workshop have been spent and */
         (sh->fSt.workShop.NSPMat == NP) &&         /* all the delivers of prime materials have been carried out and */
         (sh->fSt.workShop.NTPMat == PP*sh->fSt.workShop.NTProd);             /* all prime matrials have been turned
                                                                                                       into products */

  if (semUp (semgid, sh->access) == -1)                                                      /* exit critical region */
     { perror ("error on executing the up operation for semaphore access");
       exit (EXIT_FAILURE);
     }

  stat = true;                               /*         <---            remove this instruction for normal operation */
  return stat;
}

/**
 *  \brief Service customer operation.
 *
 *  The entrepreneur services a customer for a random generated time interval (internal operation).
 */

static void serviceCustomer (void)
{
  usleep((unsigned int) floor (20.0 * random () / RAND_MAX + 1.5));
}
