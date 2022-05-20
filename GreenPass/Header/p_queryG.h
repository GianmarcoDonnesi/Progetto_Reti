//
// Created by Gianmarco on 20/05/2022.
//

#ifndef GREENPASS_P_QUERYG_H
#define GREENPASS_P_QUERYG_H

#include "p_dbG.h"

typedef struct P_QUERYG {
    char richiesta;
    p_dbG query;
}p_queryG;


#endif

/* 	tipi di richieste:
 *	1: richiesta del ClentS e ottenimento validità greenpass
 *	2: richiesta del ClientT e ripristino o annullamento validità GreenPass
 */

/*	query: utilizzato per ottenere le informazioni dal serverV */
