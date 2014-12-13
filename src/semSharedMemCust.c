/**
 *  \file semSharedMemCust.c (implementation file)
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

/** \brief go shopping operation */
static void goShopping(unsigned int custId);

/** \brief is door open operation */
static bool isDoorOpen(unsigned int custId);

/** \brief try again later operation */
static void tryAgainLater(unsigned int custId);

/** \brief enter the shop operation */
static void enterShop(unsigned int custId);

/** \brief perusing around operation */
static unsigned int perusingAround(unsigned int custId);

/** \brief I want this operation */
static void iWantThis(unsigned int custId, unsigned int nGoods);

/** \brief exit the shop operation */
static void exitShop(unsigned int custId);
;

/** \brief end of operations customer operation */
static bool endOperCustomer(unsigned int custId);

/** \brief living normal life [internal] operation */
static void livingNormalLife(void);

/** \brief pick up [internal] operation */
static unsigned int pickUp(void);

/**
 *  \brief Main program.
 *
 *  Its role is to generate the life cycle of one of intervening entities in the problem: the customer.
 */

int main(int argc, char *argv[]) {
    int key; /*access key to shared memory and semaphore set */
    char *tinp; /* numerical parameters test flag */
    unsigned int n; /* customer identification */

    /* validation of comand line parameters */

    if (argc != 5) {
        freopen("error_GCT", "a", stderr);
        fprintf(stderr, "Number of parameters is incorrect!\n");
        exit(EXIT_FAILURE);
    } else freopen(argv[4], "w", stderr);
    n = (unsigned int) strtol(argv[1], &tinp, 0);
    if ((*tinp != '\0') || (n >= N)) {
        fprintf(stderr, "Customer process id is invalid!\n");
        exit(EXIT_FAILURE);
    }
    nFic = argv[2];
    key = (unsigned int) strtol(argv[3], &tinp, 0);
    if (*tinp != '\0') {
        fprintf(stderr, "Error on the access key communication!\n");
        exit(EXIT_FAILURE);
    }

    /* connection to the semaphore set and the shared memory region and mapping the shared region on the process address
       space */

    if ((semgid = semConnect(key)) == -1) {
        perror("error on connecting to the semaphore set");
        exit(EXIT_FAILURE);
    }
    if ((shmid = shmemConnect(key)) == -1) {
        perror("error on connecting to the shared memory region");
        exit(EXIT_FAILURE);
    }
    if (shmemAttach(shmid, (void **) &sh) == -1) {
        perror("error on mapping the shared region on the process address space");
        exit(EXIT_FAILURE);
    }

    /* simulation of the life cycle of the customer */

    unsigned int ng; /* number of selected goods */

    while (!endOperCustomer(n)) {
        while (true) {
            livingNormalLife(); /* the customer minds his own business */
            goShopping(n); /* the customer decides to visit the handicraft shop */
            if (isDoorOpen(n)) /* the customer checks if the shop door is open */
                break; /* it is */
            else tryAgainLater(n); /* it is not, the customer goes back to perform his daily chores */
        }
        enterShop(n); /* the customer enters the shop */
        ng = perusingAround(n); /* the customer inspects the offer in display and eventually picks up some goods */
        if (ng != 0)
            iWantThis(n, ng); /* the customer queues by the counter to pay for the selected goods */
        exitShop(n); /* the customer leaves the shop */
    }

    /* unmapping the shared region off the process address space */

    if (shmemDettach(sh) == -1) {
        perror("error on unmapping the shared region off the process address space");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

/**
 *  \brief Go shopping operation.
 *
 *  The customer decides to visit the handicraft shop.
 *
 *  \param custId identification of the customer
 */

static void goShopping(unsigned int custId) {
    if (semDown(semgid, sh->access) == -1) /* enter critical region */ {
        perror("error on executing the down operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    /* insert your code here */
    sh->fSt.st.custStat[custId].stat = CHECKING_SHOP_DOOR_OPEN;
    saveState(nFic,&(sh->fSt));


    if (semUp(semgid, sh->access) == -1) /* exit critical region */ {
        perror("error on executing the up operation for semaphore access");
        exit(EXIT_FAILURE);
    }
}

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

static bool isDoorOpen(unsigned int custId) {
    if (semDown(semgid, sh->access) == -1) /* enter critical region */ {
        perror("error on executing the down operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    /* insert your code here */
    
    return sh->fSt.shop.stat == SOPEN;
}

/**
 *  \brief Try again later operation.
 *
 *  The customer goes back to perform his daily chores.
 *
 *  \param custId identification of the customer
 */

static void tryAgainLater(unsigned int custId) {

    /* insert your code here */
    sh->fSt.st.custStat[custId].stat = CARRYING_OUT_DAILY_CHORES;
    saveState(nFic,&(sh->fSt));

    if (semUp(semgid, sh->access) == -1) /* exit critical region */ {
        perror("error on executing the up operation for semaphore access");
        exit(EXIT_FAILURE);
    }
}

/**
 *  \brief   Enter the shop operation.
 *
 *  The customer enters the shop.
 *
 *  \param custId identification of the customer
 */

static void enterShop(unsigned int custId) {

    /* insert your code here */
    sh->fSt.st.custStat[custId].stat = APPRAISING_OFFER_IN_DISPLAY;
    sh->fSt.shop.nCustIn++;
    saveState(nFic,&(sh->fSt));
    
    if (semUp(semgid, sh->access) == -1) /* exit critical region */ {
        perror("error on executing the up operation for semaphore access");
        exit(EXIT_FAILURE);
    }
}

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

static unsigned int perusingAround(unsigned int custId) {
    if (semDown(semgid, sh->access) == -1) /* enter critical region */ {
        perror("error on executing the down operation for semaphore access");
        exit(EXIT_FAILURE);
    }
    
    /* insert your code here */
    unsigned int nProd = 0;
    
    if (sh->fSt.shop.nProdIn > 0)
        nProd = pickUp();
    
    if(nProd != 0){
        sh->fSt.shop.nProdIn -= nProd;
        saveState (nFic, &(sh->fSt));
    }
    
    if (semUp(semgid, sh->access) == -1) /* exit critical region */ {
        perror("error on executing the up operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    return nProd;
}

/**
 *  \brief I want this operation.
 *
 *  The customer queues by the counter to pay for the selected goods.
 *
 *  \param custId identification of the customer
 *  \param nGoods number of selected goods
 */

static void iWantThis(unsigned int custId, unsigned int nGoods) {
    if (semDown(semgid, sh->access) == -1) /* enter critical region */ {
        perror("error on executing the down operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    /* insert your code here */
    sh->fSt.st.custStat[custId].stat = BUYING_SOME_GOODS;
    sh->fSt.st.custStat[custId].boughtPieces += nGoods;
    queueIn(&(sh->fSt.shop.queue), custId);

    if (semUp (semgid, sh->proceed) == -1){
        perror("error on executing the up operation for semaphore proceed");
        exit(EXIT_FAILURE);
    }
    saveState (nFic, &(sh->fSt));

    if (semUp(semgid, sh->access) == -1) /* exit critical region */ {
        perror("error on executing the up operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    /* insert your code here */
    if (semDown(semgid, sh->waitForService[custId]) == -1){
        perror("error on the executing down operation for semaphore group waitForService");
        exit(EXIT_FAILURE);
    }

}

/**
 *  \brief Exit the shop operation.
 *
 *  The customer leaves the shop.
 *
 *  \param custId identification of the customer
 */

static void exitShop(unsigned int custId) {
    if (semDown(semgid, sh->access) == -1) /* enter critical region */ {
        perror("error on executing the down operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    /* insert your code here */
    sh->fSt.st.custStat[custId].stat = CARRYING_OUT_DAILY_CHORES;
    sh->fSt.shop.nCustIn--;

    if (semUp (semgid, sh->proceed) == -1){
        perror("error on executing the up operation for semaphore proceed");
        exit (EXIT_FAILURE);
    }
    saveState (nFic, &(sh->fSt));

    if (semUp(semgid, sh->access) == -1) /* exit critical region */ {
        perror("error on executing the up operation for semaphore access");
        exit(EXIT_FAILURE);
    }
}

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

static bool endOperCustomer(unsigned int custId) {
    bool stat; /* customer status */
    unsigned int nOpCust, /* number of customers still operative */
            i; /* counting variable */

    if (semDown(semgid, sh->access) == -1) /* enter critical region */ {
        perror("error on executing the down operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    stat = (sh->fSt.workShop.nPMatIn == 0) && /* all prime materials at the workshop have been spent and */
            (sh->fSt.workShop.NSPMat == NP); /* all the delivers of prime materials have been carried out */
    if (stat) {
        nOpCust = 0; /* find out how many customers are still operative */
        for (i = 0; i < N; i++)
            if (sh->fSt.st.custStat[i].readyToWork) nOpCust += 1;
        stat = (((sh->fSt.shop.nProdIn + sh->fSt.workShop.nProdIn) < 2 * nOpCust) && /* the amount of products still */
                (nOpCust != 1)) || /* remaining to be sold is less than the number of customers still
                                                                                                   operative times 2 */
                ((sh->fSt.shop.nProdIn + sh->fSt.workShop.nProdIn + sh->fSt.workShop.NTPMat -
                PP * sh->fSt.workShop.NTProd) == 0); /* or is equal to zero */
        if (stat)
            sh->fSt.st.custStat[custId].readyToWork = false; /* the customer is signaled non operative from now on */
    }

    if (semUp(semgid, sh->access) == -1) /* exit critical region */ {
        perror("error on executing the up operation for semaphore access");
        exit(EXIT_FAILURE);
    }

    //pickUp(); /*         <---            remove this instruction for normal operation */
    //stat = true; /*         <---            remove this instruction for normal operation */
    return stat;
}

/**
 *  \brief Living normal life operation.
 *
 *  The customer minds his own business for a random generated time interval (internal operation).
 */

static void livingNormalLife(void) {
    usleep((unsigned int) floor(40.0 * random() / RAND_MAX + 1.5));
}

/**
 *  \brief Pick up operation.
 *
 *  Random generated value (internal operation).
 *
 *  \return 0, 1 or 2
 */

static unsigned int pickUp(void) {
    unsigned long val; /* auxiliary variable */

    val = (unsigned long) random();
    if ((val < (unsigned long) floor(0.3 * RAND_MAX)) || (sh->fSt.shop.nProdIn == 0)) return 0;
    else if ((val < (unsigned long) floor(0.7 * RAND_MAX)) || (sh->fSt.shop.nProdIn == 1)) return 1;
    else return 2;
}
