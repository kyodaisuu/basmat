/*

basmat - Bashicu Matrix Calculator version 2.0

Usage: basmat [-v ver] [-o opt] [-s seq] [-t stp] [-dh] ini

  ini: Initial variable in the form of BM[n].
       BM: sequence expression of Bashicu matrix.
       n: natural number. n=2 if not given.
       ex. "(0,0)(1,1)[3]"   (Quote to escape brackets in shell)
  ver: Version of Bashicu matrix system. Default = 1.
       ver = 1: The first simple version in 2014.
       ver = 2: Modified version to avoid non-terminating pattern in 2016.
  opt: Calculation option.
       opt = 1: n is constant. (Default)
       opt = 2: n = n+1 for each loop.
       opt = 3: n = n*n for each loop.
                    It matches Hardy function for ordinals below epsilon_0.
  seq: Maximum length of sequence. Default = 20.
  stp: Maximum step of calculation. Default = 0 (no limit).
   -d  Show detailed process of calculation.
   -h  Show this help and exit.

Example:
     basmat -d "(0)(1)(2)[3]"
     basmat -o 2 -s 100000 "(0,0)(1,1)[3]"
     basmat -o 4 -s 100000 "(0,0)(1,1)[3]"
     basmat -o 1 -s 13000 -t 100000 "(0,0)(1,1)[3]"

Web interface is available at
     http://gyafun.jp/ln/basmat.cgi

Author: Fish
     http://googology.wikia.com/wiki/User:Kyodaisuu
     
Version History
     http://gyafun.jp/ln/basmat/ChangeLog.txt

***********************************/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  /**************************** Initialization ****************************/
  int *S, *Delta, *C, row=0, bad, found, i=0, j, l, m, nr, len;
  int opt=1, ver=1, detail=0, help=0;
  long n, nn, k, num=0, s=20, step=0, maxstep=0;
  char *bm, arg;

  /* Read commandline options */

 while ((arg = getopt(argc, argv, "v:o:s:t:dh")) != -1) {
    switch (arg) {
        case 'v': ver=atoi(optarg); break;
        case 'o': opt=atoi(optarg); break;
        case 's': s=atoi(optarg); break;
        case 't':  maxstep=atoi(optarg); break;
        case 'd': detail=1; break;
        case 'h': help=1;
        default: /* '?' */
            help=1;
    }
  }
  
  if (ver < 1 || ver > 2) {printf("Error: invalid version.\n"); help=1;}
  if (opt < 1 || opt > 4) {printf("Error: invalid opt value.\n"); help=1;}
  if (s<0) {printf("Error: Negative value of seq not allowed.\n"); help=1;}
  if (maxstep<0) {printf("Error: Negative value of stp not allowed.\n"); help=1;}

  if (optind + 1== argc) {
    bm=argv[optind];
  } else if (optind == argc) {
    if (help==0) printf ("Error: sequence not specified.\n");
    help = 1;
  } else {
    if (help==0) printf ("Too many options.\n");
    help = 1;
  }
  
  /* Read numbers of rows */
  len = strlen( bm );
  for (m = 0; m < len; m++) {
    if (bm[m] == ',') i++;
    if (bm[m] == ')') {
      if (row < i) row = i;
      i = 0;
    }
    if (bm[m] == '[') m = len;
  }
  /* nr = Number of rows */
  nr = row+1;

  /* Allocate array */
  S = malloc(sizeof(int) * nr * s);
  Delta = malloc(sizeof(int) * nr);
  C = malloc(sizeof(int) * nr * s);
  if (S == NULL || Delta == NULL || C == NULL) {
    printf("Allocation failed.\n");
    exit(-1);
  }

  /* Read initial sequence S of Bashicu matrix */
  i = j = S[0] = 0;
  for (m = 0; m < len; m++) {
    if (bm[m] == ',') i++, S[i + j*nr]=0;
    if (bm[m] == ')') j++, i=0, S[i + j*nr]=0;
    if (bm[m] == '[') {
      for (; m < len; m++){
        k = bm[m]-'0'; if (k >= 0 && k <= 9) num = num*10+k;
      }
  }
    k = bm[m]-'0'; if (k >= 0 && k <= 9) S[i + j*nr] = S[i + j*nr]*10+k;
  }
  if ( num < 1 ) num = 2;
  if (j == 0 && help==0) {
    printf ("Error: invalid sequence of Bashicu matrix.\n");
    help=1;
  }
    
  /*** Show help and exit ***/
  if (help) {
    printf ("basmat - Bashicu Matrix Calculator version 2.0\n");
    printf ("Usage: %s [-v ver] [-o opt] [-s seq] [-t stp] [-dh] ini\n\n", argv[0]);
    printf ("  ini  Initial variable in the form of BM[n].\n"
    "       BM sequence expression of Bashicu matrix.\n"
    "       n: natural number. n=2 if not given.\n"
    "       ex. \"(0,0)(1,1)[3]\"   (Quote to escape brackets in shell)\n"
    "  ver  Version of Bashicu matrix system (1 or 2). Default = 1.\n"
    "       ver = 1: The first simple version in 2014.\n"
    "       ver = 2: Modified version to avoid non-terminating pattern in 2016.\n"
    "  opt  Calculation option.\n"
    "       opt = 1: n is constant. (Default)\n"
    "       opt = 2: n = n+1 for each loop.\n"
    "       opt = 3: n = n*n for each loop.\n"
    "       opt = 4: Simulate Hardy. n=n+1 for successor, and copy n-1.\n"
    "  seq  Maximum length of sequence. Default = 20.\n"
    "  stp  Maximum step of calculation. Default = 0 (no limit).\n\n"
    "   -d  Show detailed process of calculation.\n"
    "   -h  Show this help and exit.\n\n"
    "Web interface is available at http://gyafun.jp/ln/basmat.cgi\n");
    return 0;
  }

  /*** Initialize C matrix ***/
  for (m = 0; m<=row; m++) {
    C[m + nr] = 1;
  }

  /**************************** Start calculation ****************************/
  for (n = j-1; n >= 0; n--) {

    /* Show current sequence */
    for (i=0; i<=n; i++) {
      printf ("(");
      for (j=0; j<row; j++) printf ("%d,", S[j + i*nr]);
      printf ("%d)",S[row + i*nr]);
    }
    printf ("[%ld]\n",num);

    /******* Calculation in 3 steps*******/
    /* Step 1: Increment number with f(n) */
    if (opt == 2) num++;
    if (opt == 3) num *= num;

    /* Step 2: Determine the bad sequence and calculate Delta and C */
    bad = 0; /* Length of bad sequence */
    if (S[n*nr] == 0) {
      /* If the first row of S_n is 0, no bad sequence */
      if (opt == 4) num++;
    } else {
      if (ver == 1) {
        /***** Version 1 *****/
        /* Calculate m = Uppermost zero at the rightmost sequence */
        m = row+1; /* row = number of rows + 1 */
        for (j=0; j<=row; j++){
          if (S[j + n*nr] == 0) m=j, j=row+1;
        }
        /* Determine the bad sequence and calculate Delta */
        for (k=0; k<=n; k++) {
          found = 1; for (l = 0; l < m; l++) if (S[l + (n-k)*nr] >= S[l + n*nr]) found=0;
          /* If found, calculate Delta */
          if (found) {
            for (l = 0; l<=row; l++) Delta[l] = (l < m-1)? S[l + n*nr]-S[l + (n-k)*nr]: 0;
            bad = k; k = n+1;
          }
        }
      } else {
        /***** Version 2 *****/
        /* Clear Delta */
        for (m=0; m<=row; m++) Delta[m]=0;
        /* Determine the bad sequence and calculate Delta */
        for (k=0; k<=n; k++) {
          for (l = 0; l <= row; l++) {
             if (S[l + (n-k)*nr] < S[l + n*nr] - Delta[l]) {
               if (S[l+1 + n*nr] == 0 || l==row) {
                 l = row; bad=k; k=n;
               } else {
                 Delta[l] = S[l + n*nr] - S[l + (n-k)*nr];
               }
             } else {
               l = row; /* Go to left sequence (k loop) */
             }
          }
        }
        /* Calculate C matrix */
        for (k=1; k<=bad; k++) {
           /* Find the largest l which satisfies S_(n-bad+l)[0] < S_(n-bad+k)[0] */
           for (l=k; l>=0; l--) {
              if (S[(n-bad+l)*nr] < S[(n-bad+k)*nr]) {
                 for (m=0; m<=row; m++) {
                    C[m+(k+1)*nr] = ( S[m+(n-bad)*nr] < S[m+(n-bad+k)*nr] && C[m+(l+1)*nr] == 1 )? 1: 0;
                 }
                 l=0;
              }
           }
        }
      }
    }

    /* Step 3: Copy bad sequence */
    /* If opt=4, copy n-1; otherwise, copy n. nn = next n */
    nn = (opt == 4)? n+bad*(num-1): n+bad*num;
    /* Check termination */
    if (nn > s) {
      printf ("Length of sequence exceeds %ld at next step.\n",s);
      free(S); free(Delta); return 0;
    }
    if (maxstep && step++ >= maxstep) {
      printf ("Maximum step of calculation %ld has reached.\n",maxstep);
      free(S); free(Delta); return 0;
    }
    
    /* Show detailed process (commandline option -d) */
    if (detail && bad) {
       printf ("G = ");
       for (l=0; l<n; l++) {
          if (l==n-bad) {
            if (l==0) printf("empty");
            printf ("\nB = ");
          }
          printf ("(");
          for (m=0; m<row; m++) printf ("%d,",S[m+l*nr]);
          printf ("%d)",S[row+l*nr]);
       }
       printf ("\nDelta = (");
       for (l=0; l<row; l++) printf ("%d,",Delta[l]);
       printf ("%d)\n", Delta[row]);
        /* Show C matrix */
        if (ver > 1) {
            printf ("C = ");
            for (l=1; l<=bad; l++) {
                printf ("(");
                for (m=0; m<row; m++) printf ("%d,",C[m+l*nr]);
                printf ("%d)",C[row+l*nr]);
            }
            printf ("\n");
        }
        /* Show f(n) */
        printf ("f(n) = %ld\n",num);                    
    }

    /* Copy bad sequence */
    if (ver == 1) {
    /***** Version 1 *****/
      while (n < nn) {
        for (l=0; l<=row; l++) S[l + n*nr] = S[l + (n-bad)*nr] + Delta[l];
        n++;
      }
    } else {
      /***** Version 2 *****/
      m = 1;
      while (n < nn) {
        for (l=0; l<=row; l++) {
          S[l + n*nr] = S[l + (n-bad)*nr] + Delta[l] * C[l + m*nr];
        }
        m++; n++;
        if (m > bad) m=1;
      }
    }

  }
  /* Finished calculation */
  printf ("Finished. Calculated number = %ld\n",num);
  free(S); free(Delta); free(C); return 0;
}
