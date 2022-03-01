#include "parserDef.h"
#include <stdio.h>

#define EXCLUDE_EPS -288230376151711745L
#define INCLUDE_EPS 288230376151711744L

void first(Grammar g, rhsCharNode rcn, long int* firstBitString)

void computeFirst(Grammar g, nonTerminal nt, long int* firstBitString) {
    lhsChar lc = g[nt];
    for (int i=0; i<lc->numRules; i++) {
        first(lc->heads[i], firstBitString);
    }
}

void first(Grammar g, rhsCharNode rcn, long int* firstBitString) {
    if (rcn->tag==0) { //symbol is a terminal
        *firstBitString = *firstBitString | 1<<(rcn->s.t);
    } else { //symbol is a non-terminal => further digging needed
        lhsChar lc = g[rcn->s.nt];
        if (lc->first == 0) { //first set isn't initialized yet
            computeFirst(g, rcn->s.nt, &(lc->first));
        }
        
        if (lc->isNullable) {
            *firstBitString = *firstBitString | (lc->first & EXCLUDE_EPS); //exclude eps
            if (rcn->next != NULL)
                first(g, rcn->next, firstBitString);
            else
                //This was the last symbol in the rule
                *firstBitString = *firstBitString | INCLUDE_EPS;            
        } else {
            //no need to compute first sets of further nonTerminals in the rule
            *firstBitString = *firstBitString | (lc->first);
        }
    }
    return;
}