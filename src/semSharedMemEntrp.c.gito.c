char appraiseSit(void) {
    if ((statusEntrep = pthread_mutex_lock(&accessCR)) != 0) /* enter monitor */ {
        errno = statusEntrep; /* save error in errno */
        perror("error on entering monitor(EP)");
        statusEntrep = EXIT_FAILURE;
        pthread_exit(&statusEntrep);
    }
    pthread_once(&init, initialization); /* internal data initialization */
    /* insert your code here */
    char action;
    while (true) {
        while (!nWakeUp) { // if no thread is pending, craftsman or customer
            if ((statusEntrep = pthread_cond_wait(&proceed, &accessCR)) != 0) { // freeze the entrepreneur
                errno = statusEntrep; /* save error in errno */
                perror("error: wait to freeze the entrepreneur failed");
                statusEntrep = EXIT_FAILURE;
                pthread_exit(&statusEntrep);
            }
        }
        nWakeUp--; // lets go to some customer or crafstman
        if (!queueEmpty(&(fSt.shop.queue))) { // if not empty, so there a client at the counter
            action = 'C';
            break;
        }
        if (fSt.shop.primeMatReq) { // craftsman asking for prime materials
            action = 'P';
            break;
        }
        if (fSt.shop.prodTransfer) { // craftsman telling to go get products
            action = 'G';
            break;
        }
        if ((fSt.shop.nCustIn == 0) && /* the shop has no customers in and */
                (fSt.shop.nProdIn == 0) && /* all products in display have been sold and */
                !fSt.shop.primeMatReq && /* no craftsman has phoned to request prime materials or */
                !fSt.shop.prodTransfer && /* to ask for a batch of products to be collected) and */
                (fSt.workShop.nProdIn == 0) && /* there are no finished products in the storeroom at the workshop and */
                (fSt.workShop.nPMatIn == 0) && /* all prime materials at the workshop have been spent and */
                (fSt.workShop.NTPMat == PP * fSt.workShop.NTProd) && /* all prime matrials have been turned into products and */
                (fSt.workShop.NSPMat == NP)) { /* all the delivers of prime materials have been carried out */
            action = 'E';
            break;
        }
    }