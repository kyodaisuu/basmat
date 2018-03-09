/*

basmat.c : Bashicu Matrix Calculator

Usage: basmat ini [max] [opt] [step]

ini: Initial variable in the form of BM[n], where BM is a sequence expression
     of Bashicu matrix, and n is a natural number. n=2 if not given.
max: Maximum length of sequence. Default is 20.
opt: Calculation option. Default is 1.
     opt = 1: n is constant.
     opt = 2: n = n+1 for each loop.
     opt = 3: n = n*n for each loop.
     opt = 4: Simulate Hardy. n=n+1 for successor, and copy n-1.
              It matches Hardy function for ordinals below epsilon_0.
step: Maximum step of calculation. Default is 0 (no limit).

Example:
     basmat "(0,0)(1,1)]3]" 100000 2
     basmat "(0,0)(1,1)[3]" 10000000 4
     basmat "(0,0)(1,1)[3]" 13000 1 100000

Web interface is available at
     http://gyafun.jp/ln/basmat.cgi

Author: Fish
     http://googology.wikia.com/wiki/User:Kyodaisuu

Created on Sep. 1, 2014.

***********************************/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int *a, *b, c=0, g, h, i=0, j, l, m, y, len, opt;
  long f, k, n=0, z, step=0, maxstep;
  char *bm;

  /* Read parameters */
  if (argc < 2) {
    printf ("Usage: %s ini [max] [opt] [step]\n", argv[0]);
    printf ("See comment in the source: http://gyafun.jp/ln/basmat.cgi?ini=s\n");
    return 0;
  }
  bm  = argv[1];
  z = (argc > 2)? atol(argv[2]): 20;
  if (z < 1) z = 1;
  opt = (argc > 3)? atoi(argv[3]): 1;
  maxstep = (argc > 4)? atoi(argv[4]): 0;
  if (maxstep < 0) maxstep = 0;

  /* Read numbers of rows */
  len = strlen( bm );
  for (m = 0; m < len; m++) {
    if (bm[m] == ',') i++;
    if (bm[m] == ')') {
      if (c < i) c = i;
      i = 0;
    }
    if (bm[m] == '[') m = len;
  }
  y = c+1;

  /* Allocate array */
  a = malloc(sizeof(int) * y * z);
  b = malloc(sizeof(int) * y);
  if (a == NULL || b == NULL) {
    printf("Allocation failed.\n");
    exit(-1);
  }

  /* Read initial variable */
  i = j = a[0] = 0;
  for (m = 0; m < len; m++) {
    if (bm[m] == ',') i++, a[i + j*y]=0;
    if (bm[m] == ')') j++, i=0, a[i + j*y]=0;
    if (bm[m] == '[') {
      for (; m < len; m++){
        k = bm[m]-'0'; if (k >= 0 && k <= 9) n = n*10+k;
      }
    }
    k = bm[m]-'0'; if (k >= 0 && k <= 9) a[i + j*y] = a[i + j*y]*10+k;
  }
  if ( n < 1 ) n = 2;
  if (j == 0) {
    printf ("Invalid initial parameter.\n");
    return 0;
  }

  /*** Start calculation ***/
  for (f=j-1; f >= 0; f--) {

    /* Show current sequence */
    for (i=0; i<=f; i++) {
      printf ("(");
      for (j=0; j<c; j++) printf ("%d,", a[j + i*y]);
      printf ("%d)",a[c + i*y]);
    }
    printf ("[%ld]\n",n);

    /* Calculation */
    if (opt == 2) n++;
    if (opt == 3) n *= n;
    if (a[f*y] == 0) {
      g = 0;
      if (opt == 4) n++;
    } else {
      m = c+1; g = 0;
      for (j=0; j<=c; j++){
        if (a[j + f*y] == 0) m=j, j=c+1;
      }
      for (k=0; k<=f; k++) {
        h = 1; for (l = 0; l < m; l++) if (a[l + (f-k)*y] >= a[l + f*y]) h=0;
        if (h) {
          for (l = 0; l<=c; l++) b[l] = (l < m-1)? a[l + f*y]-a[l + (f-k)*y]: 0;
          g = k; k = f+1;
        }
      }
    }
    k = (opt == 4)? f+g*(n-1): f+g*n;
    if (k > z) {
      printf ("Length of sequence exceeds %ld at next step.\n",z);
      free(a); free(b); return 0;
    }
    if (maxstep && step++ >= maxstep) {
      printf ("Maximum step of calculation %ld has reached.\n",maxstep);
      free(a); free(b); return 0;
    }
    while (f < k) {
      for (l=0; l<=c; l++) a[l + f*y] = a[l + (f-g)*y] + b[l];
      f++;
    }
  }
  printf ("Finished. n = %ld\n",n);
  free(a); free(b); return 0;
}
