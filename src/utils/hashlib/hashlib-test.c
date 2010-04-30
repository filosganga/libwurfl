/* -------------- File hashtest.c ------------------ */

/* Added test 8, not in testsuite, to verify all
   storage properly freed with /DMALLDBG.  Only for
   my nmalloc/malldbg system for DJGPP
   2003-5-31 cbf.
*/

#include <stdio.h>
#include <stdlib.h>
#include "hashlib.h"
#include "cokusmt.h"

#ifdef MALLDBG
#  include "malldbg.h"
#endif

typedef struct t1tag {
   unsigned long  value;
   unsigned long  count;
   unsigned long  timesfound;
   struct t1tag  *next;
} t1item, *t1itemptr;

/* 1------------------1 */

unsigned long t1hash(void *item)
{
   t1itemptr t1p = item;

   return t1p->value;
} /* t1hash */

/* 1------------------1 */

unsigned long t1rehash(void *item)
{
   t1itemptr t1p = item;

   return t1p->value >> 3;
} /* t1rehash */

/* 1------------------1 */

int t1cmp(void *litem, void *ritem)
{
   t1itemptr t1lp = litem,
             t1rp = ritem;

   if     (t1lp->value == t1rp->value) return 0;
   else if (t1lp->value > t1rp->value) return 1;
   else                                return -1;
} /* t1cmp */

/* 1------------------1 */

void *t1dupe(void *item)
{
   t1itemptr t1p = item,
             tdupe;

   if ((tdupe = malloc(sizeof *tdupe))) *tdupe = *t1p;
   return tdupe;
} /* t1dupe */

/* 1------------------1 */

void t1undupe(void *item)
{
   free(item);
} /* t1undupe */

/* 1------------------1 */

int show(void* stored, void* datum, void* xtra)
{
   t1itemptr t1p = stored;

   (void) datum; /* anti unused */
   if (xtra)
      printf("%10lu ct = %lu, i = %d\n",
              t1p->value, t1p->count, *(int*)xtra);
   else
      printf("%10lu ct = %lu\n", t1p->value, t1p->count);
   return 0;
} /* show */

/* 1------------------1 */

/* First test - inject 11 random values */
void dotest1(void)
{
   int       i, j, err;
   t1item    item, itemsaved;
   t1itemptr stored;
   hshtbl   *h;

   printf("HASHLIB test01\n");
   for (j = 0; j < 2; j++) {
      h = hshinit(t1hash,            /* hash function */
                 t1rehash,           /* rehash function */
                 t1cmp,              /* compare function */
                 t1dupe,             /* dupe function */
                 t1undupe,           /* hshfree function */
                 1);                 /* use debug output */

      printf("\nNew table, inserting\n");
      item.count = 0;
      for (i = 0; i < 3; i++) {
        item.value = randomMT();
        if ((stored = hshinsert(h, &item))) {
           stored->count++;
           if ((stored = hshinsert(h, &item))) {
              stored->count++;
           }
           else printf("Store failure, i = %d\n", i);
        }
        else printf("Store failure, i = %d\n", i);
      }
      itemsaved = item;

      for (i = 3; i < 7; i++) {
        item.value = randomMT();
        if ((stored = hshinsert(h, &item))) {
           stored->count++;
        }
        else printf("Store failure, i = %d\n", i);
      }

      /* One of the two find tests requires a rehash and */
      /* the other doesn't.  Don't diddle the sequence   */
      /* This test is highly dependent on the internal   */
      /* details of hashlib such as INITSZ, and randomMT */

      if ((stored = hshfind(h, &itemsaved))) {
         printf("Found: ");
         show(stored, NULL, NULL);
      }
      else {
         printf("Not found: ");
         show(&itemsaved, NULL, NULL);
      }

      itemsaved.value = randomMT();
      if ((stored = hshfind(h, &itemsaved))) {
         printf("found: ");
         show(stored, NULL, NULL);
      }
      else {
         printf("Not found: ");
         show(&itemsaved, NULL, NULL);
      }

      printf("\nWalking\n");
      err = hshwalk(h, show, NULL);
      printf("returned %d\n", err);

      hshkill(h);
   }
} /* dotest1 */

/* 1------------------1 */

/* Second test - inject 20 sequential values */
/* Uses the same subroutines as test 1       */
void dotest2(void)
{
   int       i, j, err;
   t1item    item, itemsaved;
   t1itemptr stored;
   hshtbl   *h;

   printf("HASHLIB test02\n");
   for (j = 0; j < 2; j++) {
      h = hshinit(t1hash,            /* hash function */
                 t1rehash,           /* rehash function */
                 t1cmp,              /* compare function */
                 t1dupe,             /* dupe function */
                 t1undupe,           /* hshfree function */
                 1);                 /* use debug output */

      printf("\nNew table, inserting\n");
      item.count = 0;
      for (i = 1; i < 4; i++) {
        item.value = i + j * 10;
        if ((stored = hshinsert(h, &item))) {
           stored->count++;
           if ((stored = hshinsert(h, &item))) {
              stored->count++;
           }
           else printf("Store failure, i = %d\n", i);
        }
        else printf("Store failure, i = %d\n", i);
      }
      itemsaved = item;

      for (i = 4; i < 11; i++) {
        item.value = i + j * 10;
        if ((stored = hshinsert(h, &item))) {
           stored->count++;
        }
        else printf("Store failure, i = %d\n", i);
      }

      if ((stored = hshfind(h, &itemsaved))) {
         printf("Found: ");
         show(stored, NULL, NULL);
      }
      else {
         printf("Not found: ");
         show(&itemsaved, NULL, NULL);
      }

      itemsaved.value = 0;
      if ((stored = hshfind(h, &itemsaved))) {
         printf("found: ");
         show(stored, NULL, NULL);
      }
      else {
         printf("Not found: ");
         show(&itemsaved, NULL, NULL);
      }

      printf("\nWalking\n");
      err = hshwalk(h, show, NULL);
      printf("returned %d\n", err);

      hshkill(h);
   }
} /* dotest2 */

/* 1------------------1 */

static void showstate(hshtbl *m)
{
   hshstats hs;

   hs = hshstatus(m);
   printf("Status: Entries=%lu, Deleted=%lu, Probes=%lu, Misses=%lu",
           hs.hentries, hs.hdeleted, hs.probes, hs.misses);
   if (hs.herror) {
      if (hs.herror & hshNOMEM)   printf(", NOMEM");
      if (hs.herror & hshTBLFULL) printf(", TBLFULL");
      if (hs.herror & hshINTERR)  printf(", INTERR");
   }
   printf("\n");
} /* showstate */

/* 1------------------1 */

/* Third test - inject sequential values until full */
/* Uses the same subroutines as test 1              */
void dotest3(void)
{
   int       i, err;
   t1item    item;
   hshtbl   *h;

   printf("HASHLIB test03\n");
   h = hshinit(t1hash,             /* hash function */
               t1rehash,           /* rehash function */
               t1cmp,              /* compare function */
               t1dupe,             /* dupe function */
               t1undupe,           /* hshfree function */
               1);                 /* use debug output */

   printf("\nNew table, inserting\n");
   item.count = 0;
   i = 0;
   do {
      i++;
      item.value = randomMT();
      printf("%2d: %10lu\n", i, item.value);
   } while ((i < 17) && hshinsert(h, &item));
   if (i < 17) printf("Store failure, i = %d\n", i);
   else printf("Reorganized\n");

   showstate(h);

   printf("\nWalking\n");
   err = hshwalk(h, show, NULL);
   printf("returned %d\n", err);

   hshkill(h);
} /* dotest3 */

/* 1------------------1 */

#define HISTMAX 9

typedef struct histogram {
   int   ixsz;
   int   sizes[HISTMAX+1];
   int   found[HISTMAX+1];
} histogram, *histogramptr;

/* 1------------------1 */

/* Scan the distribution of count and timesfound fields */
int getcounts(void *stored, void *datum, void *xtra)
{

   t1itemptr    t1p   = stored;
   histogramptr hgptr = datum;

   (void) xtra; /* anti unused warn */
   if (t1p->count > HISTMAX) ++(hgptr->sizes[HISTMAX]);
   else ++(hgptr->sizes[t1p->count]);

   if (t1p->timesfound > HISTMAX) ++(hgptr->found[HISTMAX]);
   else ++(hgptr->found[t1p->timesfound]);

   return 0;
} /* getcounts */

/* 1------------------1 */

void showinserts(histogramptr hgp)
{
   int i;

   for (i = 0; i < HISTMAX; i++) {
      printf("%8d items were inserted %d times\n",
             hgp->sizes[i], i);
   }
   /* leaving i == HISTMAX */
   printf("%8d items were inserted %d or more times\n",
           hgp->sizes[i], i);
} /* showinserts */

/* 1------------------1 */

void showfinds(histogramptr hgp)
{
   int i;

   for (i = 0; i < HISTMAX; i++) {
      printf("%8d items were found %d times\n",
             hgp->found[i], i);
   }
   /* leaving i == HISTMAX */
   printf("%8d items were found %d or more times\n",
           hgp->found[i], i);
} /* showfinds */

/* 1------------------1 */

void clearhistogram(histogramptr hgp)
{
   int i;

   for (i = 0; i <= HISTMAX; i++) {
      hgp->sizes[i] = 0;
      hgp->found[i] = 0;
   }
   hgp->ixsz = HISTMAX;
} /* clearhistogram */

/* 1------------------1 */

/* Fourth test - inject multiple values, show distribution */
/* Uses the same subroutines as test 1 + getcounts         */
void dotest4(unsigned long p)
{
   int           err;
   unsigned long i;
   t1item        item, *stored;
   hshtbl       *h;
   histogram     dtag;

   printf("HASHLIB test04\n");
   h = hshinit(t1hash,             /* hash function */
               t1rehash,           /* rehash function */
               t1cmp,              /* compare function */
               t1dupe,             /* dupe function */
               t1undupe,           /* hshfree function */
               1);                 /* use debug output */

   if (!p) p = 10000;
   printf("\nNew table, inserting %lu values\n", p);
   item.count = item.timesfound = 0;
   for (i = 0; i < p; i++) {
      item.value = randomMT();
      stored = hshinsert(h, &item);
      stored->count++;
      if (0 == (i % 20000)) {
         printf("\r%lu inserted", i);
         fflush(stdin);
      }
   }
   putchar('\n');
   showstate(h);

   printf("\nWalking ");
   clearhistogram(&dtag);
   err = hshwalk(h, getcounts, &dtag);
   printf("returned %d\n", err);

   showinserts(&dtag);

   if (p & 1) printf("Suppressing hshkill()\n");
   else hshkill(h);
} /* dotest4 */

/* 1------------------1 */

/* This should avoid creating a perfect hash of the input data */
/* http://www.concentric.net/~Ttwang/tech/inthash.htm          */
/* Jenkin's 32 bit mix                                         */
unsigned long t5hash(void *item)
{
   t1itemptr t1p = item;
   unsigned long work;

   work = t1p->value;
   work += ~(work << 15);
   work ^= ~(work << 22);
   work += ~(work << 4);
   work ^= ~(work << 9);
   work += ~(work << 10);
   work ^= ~(work << 2);
   work += ~(work << 7);
   work ^= ~(work << 12);
   return work;
} /* t5hash */

/* 1------------------1 */

#define RANGE 4000

/* 1------------------1 */

/* Fifth test - inject multiple values and confirm that the   */
/* results do not depend on the hash function, althougth the  */
/* statistics do.                                             */
/* Here item is restricted, so that multiple values are found */
/* Now we have to change the hash function, since the value   */
/* itself results in a perfect hash over the table size.      */
void dotest5(unsigned long p)
{
   int           err;
   unsigned long i;
   t1item        item, *stored;
   hshtbl       *h;
   histogram     dtag;

   printf("HASHLIB test05\n");
   h = hshinit(t5hash,             /* hash function */
               t1rehash,           /* rehash function */
               t1cmp,              /* compare function */
               t1dupe,             /* dupe function */
               t1undupe,           /* hshfree function */
               1);                 /* use debug output */

   if (!p) p = 10000;
   seedMT(4357U);
   printf("\nNew table, inserting %lu values range 0..%d\n",
                                   p,             RANGE-1);
   item.count = item.timesfound = 0;
   for (i = 0; i < p; i++) {
      item.value = randomMT() % RANGE;
      stored = hshinsert(h, &item);
      stored->count++;
   }
   showstate(h);

   printf("\nWalking ");
   clearhistogram(&dtag);
   err = hshwalk(h, getcounts, &dtag);
   printf("returned %d\n", err);

   showinserts(&dtag);

   printf("This table has to be killed for new hash function\n");
   hshkill(h);

   h = hshinit(t1hash,             /* hash function */
               t1rehash,           /* rehash function */
               t1cmp,              /* compare function */
               t1dupe,             /* dupe function */
               t1undupe,           /* hshfree function */
               1);                 /* use debug output */

   seedMT(4357U);
   printf("\nNew table, inserting same values\n");
   item.count = item.timesfound = 0;
   for (i = 0; i < p; i++) {
      item.value = randomMT() % RANGE;
      stored = hshinsert(h, &item);
      stored->count++;
   }
   showstate(h);

   printf("\nWalking ");
   clearhistogram(&dtag);
   err = hshwalk(h, getcounts, &dtag);
   printf("returned %d\n", err);

   showinserts(&dtag);

   if (p & 1) printf("Suppressing hshkill()\n");
   else hshkill(h);
} /* dotest5 */

/* 1------------------1 */

/* Sixth test - inject multiple values and search */
/* Here item is restricted, so that multiple values are found */
/* Now we have to change the hash function, since the value   */
/* itself results in a perfect hash over the table size. The  */
/* result never rehashes during a find, and is peculiar.      */
void dotest6(unsigned long p)
{
   int           err;
   unsigned long i;
   t1item        item, *stored;
   hshtbl       *h;
   histogram     dtag;

   printf("HASHLIB test06\n");
   h = hshinit(t5hash,             /* hash function */
               t1rehash,           /* rehash function */
               t1cmp,              /* compare function */
               t1dupe,             /* dupe function */
               t1undupe,           /* hshfree function */
               1);                 /* use debug output */

   if (!p) p = 10000;
   printf("\nNew table, inserting %lu values range 0..%d\n",
                                   p,              RANGE-1);
   item.count = item.timesfound = 0;
   for (i = 0; i < p; i++) {
      item.value = randomMT() % RANGE;
      stored = hshinsert(h, &item);
      stored->count++;
   }
   showstate(h);

   printf("\nWalking ");
   clearhistogram(&dtag);
   err = hshwalk(h, getcounts, &dtag);
   printf("returned %d\n", err);

   showinserts(&dtag);

   printf("\nFinding %lu values in range 0..%d\n", p, 2*RANGE-1);
   /* Note that this is a different set of values */
   for (i = 0; i < p; i++) {
      item.value = randomMT() % (2*RANGE);
      stored = hshfind(h, &item);
      if (stored) stored->timesfound++;
   }
   showstate(h);

   printf("\nWalking ");
   clearhistogram(&dtag);
   err = hshwalk(h, getcounts, &dtag);
   printf("returned %d\n", err);

   showfinds(&dtag);

   if (p & 1) printf("Suppressing hshkill()\n");
   else hshkill(h);
} /* dotest6 */

/* 1------------------1 */

/* Seventh test - inject values, delete, inject   */
/* variant on test 1, using deletion              */
/* This test uses knowledge of hashlib thresholds */
void dotest7(void)
{
   int       i, err;
   t1item    item, itemsaved;
   t1itemptr stored;
   hshtbl   *h;

   printf("HASHLIB test07\n");
   h = hshinit(t1hash,            /* hash function */
              t1rehash,           /* rehash function */
              t1cmp,              /* compare function */
              t1dupe,             /* dupe function */
              t1undupe,           /* hshfree function */
              1);                 /* use debug output */

   printf("\nNew table, inserting\n");
   item.count = 0;
   for (i = 0; i < 3; i++) {
     item.value = randomMT();
     if ((stored = hshinsert(h, &item))) {
        stored->count++;
        if ((stored = hshinsert(h, &item))) {
           stored->count++;
        }
        else printf("Store failure, i = %d\n", i);
     }
     else printf("Store failure, i = %d\n", i);
   }
   itemsaved = item;

   for (i = 3; i < 15; i++) {
      item.value = randomMT();
      if ((stored = hshinsert(h, &item))) {
         stored->count++;
      }
      else printf("Store failure, i = %d\n", i);
   }

   printf("\nWalking\n");
   err = hshwalk(h, show, NULL);
   printf("returned %d\n", err);
   showstate(h);

   /* One of the two delete tests requires a rehash and */
   /* the other doesn't.  Don't diddle the sequence     */
   /* This test is highly dependent on the internal     */
   /* details of hashlib such as INITSZ, and randomMT   */

   if ((stored = hshdelete(h, &itemsaved))) {
      printf("Deleted: ");
      show(stored, NULL, NULL);
   }
   else {
      printf("Not found, not deleted: ");
      show(&itemsaved, NULL, NULL);
   }

   itemsaved.value = randomMT();
   if ((stored = hshdelete(h, &itemsaved))) {
      printf("deleted: ");
      show(stored, NULL, NULL);
   }
   else {
      printf("Not found, not deleted: ");
      show(&itemsaved, NULL, NULL);
   }

   printf("\nWalking\n");
   err = hshwalk(h, show, NULL);
   printf("returned %d\n", err);
   showstate(h);

   /* Now we have the table nearly full - delete some */
   seedMT(4357U);       /* regenerate the same items */
   for (i = 0; i < 6; i++) { /* one already deleted */
      item.value = randomMT();
      if (!(stored = hshdelete(h, &item))) {
         printf("Not found, not deleted: ");
         show(&item, NULL, NULL);
      }
   }
   printf("\nWalking\n");
   err = hshwalk(h, show, NULL);
   printf("returned %d\n", err);
   showstate(h);

   /* skip the used values from randomMT, last is new */
   for (i = 7; i < 17; i++) item.value = randomMT();
   /* this insertion should reorganize */
   if ((stored = hshinsert(h, &item)))
      stored->count++;

   printf("\nWalking\n");
   err = hshwalk(h, show, NULL);
   printf("returned %d\n", err);
   showstate(h);

   hshkill(h);
} /* dotest7 */

/* 1------------------1 */

#ifdef MALLDBG
/* Variant on test 5, monitoring heap status */
/* USE ONLY with nmalloc/malldbg package     */
void dotest8(unsigned long p)
{
   int             err;
   unsigned long   i;
   t1item          item, *stored;
   hshtbl         *h;
   histogram       dtag;
   struct mallinfo mi1, mi2, mi3;

   printf("HASHLIB test08\n");
   malloc_debug(1);     /* enable everything */
   mi1 = mallinfo();    /* status before */
   h = hshinit(t5hash,             /* hash function */
               t1rehash,           /* rehash function */
               t1cmp,              /* compare function */
               t1dupe,             /* dupe function */
               t1undupe,           /* hshfree function */
               1);                 /* use debug output */

   if (!p) p = 10000;
   seedMT(4357U);
   printf("\nNew table, inserting %lu values range 0..%d\n",
                                   p,               RANGE-1);
   item.count = item.timesfound = 0;
   for (i = 0; i < p; i++) {
      item.value = randomMT() % RANGE;
      stored = hshinsert(h, &item);
      stored->count++;
   }
   showstate(h);

   printf("\nWalking ");
   clearhistogram(&dtag);
   err = hshwalk(h, getcounts, &dtag);
   printf("returned %d\n", err);

   showinserts(&dtag);
   mi2 = mallinfo();  /* status during, after filling table */

   puts("Hshkill should restore heap state\n");
   hshkill(h);
   mi3 = mallinfo();  /* status after, table destroyed */

   printf("                 before   during    after\n");
   printf("Space allocated  %6d %8d %8d\n",
           mi1.uordblks, mi2.uordblks, mi3.uordblks);
   printf("Free blocks      %6d %8d %8d\n",
           mi1.hblks, mi2.hblks, mi3.hblks);
   printf("Total blocks     %6d %8d %8d\n",
           mi1.ordblks, mi2.ordblks, mi3.ordblks);

} /* dotest8 */
#endif

/* 1------------------1 */

/* Test the hash library system */
/*
int main(int argc, char ** argv)
{
   int           t;
   unsigned long p;

   if (argc < 2) printf("Usage: %s test_num [count]\n", argv[0]);
   else {
     if (argc > 2) p = strtoul(argv[2], NULL, 0);
     else p = 0;
     t = strtoul(argv[1], NULL, 0);
     switch (t) {
case 1: dotest1(); break;
case 2: dotest2(); break;
case 3: dotest3(); break;
case 4: dotest4(p); break;
case 5: dotest5(p); break;
case 6: dotest6(p); break;
case 7: dotest7(); break;
#ifdef MALLDBG
case 8: dotest8(p); break;
#endif
default:
        puts("Invalid test num\n");
        printf("Usage: %s test_num [count]\n", argv[0]);
        puts("Test Use\n"
             "  1  Inject random values\n"
             "  2  Inject 20 sequential values\n"
             "  3  Inject 17 values, check reorg\n"
             "  4  Inject count values, show distribution\n"
             "  5  Like 4, using different hash functions\n"
             "     Confirm net results function independent\n"
             "  6  Inject count values and search\n"
             "  7  Check function of delete operation\n"
#ifdef MALLDBG
             "  8  Check memory allocations freed on hshkill\n"
#endif
             "\n4 thru 6, odd counts suppress final hshkill"
            );
        return EXIT_FAILURE;
     }
   }
   fprintf(stderr, "\a\n");
   return 0;
}  main
*/

/* -------------- File hashtest.c ------------------ */
