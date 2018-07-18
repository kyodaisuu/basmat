/*

   basmat - Bashicu Matrix Calculator
*/
const char *version = "3.1-beta";
/*

   Project website
   https://kyodaisuu.github.io/basmat/

   Online Calculator
   http://gyafun.jp/ln/basmat.cgi

**********************************************/

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *versionBM = "2"; /* Default version of BM */

/************************ Hep ***********************/

/*
   showHelp - show help
*/

void showHelp() {
  printf("basmat - Bashicu Matrix Calculator version %s\n", version);
  printf(
      "Usage: basmat [-v ver] [-o opt] [-s seq] [-t stp] [-dh] ini\n\n"
      "  ini  Initial variable in the form of BM[n].\n"
      "       BM sequence expression of Bashicu matrix.\n"
      "       n: natural number. n=2 if not given.\n"
      "       ex. \"(0,0)(1,1)[3]\"   (Quote to escape brackets in shell)\n"
      "  ver  Version of Bashicu matrix system. Default = %s.\n",
      versionBM);
  printf(
      "       Available versions: 1, 2, 2.1, 2.2, 2.3, 3\n"
      "  opt  Calculation option.\n"
      "       opt = 1: n is constant. (Default)\n"
      "       opt = 2: n = n+1 for each loop.\n"
      "       opt = 3: n = n*n for each loop.\n"
      "       opt = 4: Simulate Hardy. n=n+1 for successor, and copy n-1.\n"
      "  seq  Maximum length of sequence. Default = 20.\n"
      "  stp  Maximum step of calculation. Default = 0 (no limit).\n"
      "   -d  Show detailed process of calculation.\n"
      "   -T  Test for development.\n"
      "   -h  Show this help and exit.\n\n"
      "Web interface is available at http://gyafun.jp/ln/basmat.cgi\n"
      "See also https://kyodaisuu.github.io/basmat/\n");
  return;
}

/*
  intVersion - get version as integer

*/

int intVersion(char *version) {
  if (strcmp(version, "1.0") == 0 || strcmp(version, "1") == 0)
    return 100;
  else if (strcmp(version, "2.0") == 0 || strcmp(version, "2") == 0)
    return 200;
  else if (strcmp(version, "2.1") == 0)
    return 210;
  else if (strcmp(version, "2.2") == 0)
    return 220;
  else if (strcmp(version, "2.3") == 0)
    return 230;
  else if (strcmp(version, "3.0") == 0 || strcmp(version, "3") == 0)
    return 300;
  else {
    return 0;
  }
}

/*************************** Input ***********************/

/* getMatrix(bm, S, &nc, &nr, &num);
   Input variables:
     bm = Bashicu matrix as input string
     S  = Just allocate memory
     nr = numbers of rows
   Return variables:
     S  = Bashicu matrix as array of integer
     nc = numbers of columns
     num = Integer as a parameter of BM */

void getMatrix(char *bm, int *S, int nr, long *nc, long *num) {
  int len, i, j, m;
  long k;

  len = strlen(bm);
  i = j = S[0] = 0;
  for (m = 0; m < len; m++) {
    if (bm[m] == ',') i++, S[i + j * nr] = 0;
    if (bm[m] == ')') j++, i = 0, S[i + j * nr] = 0;
    if (bm[m] == '[') {
      for (; m < len; m++) {
        k = bm[m] - '0';
        if (k >= 0 && k <= 9) *num = *num * 10 + k;
      }
    }
    k = bm[m] - '0';
    if (k >= 0 && k <= 9) S[i + j * nr] = S[i + j * nr] * 10 + k;
  }
  *nc = j;
  return;
}

/*
   get Bashicu matrix as a sequence (inverse of getMatrix)

*/

char *getSeq(int *S, int nr, long nc) {
  int i, j;
  int digit = (int)log10(nc) + 1;
  int mem = nc * (nr * (digit + 1) + 2) + 5;
  char *seq = malloc(mem);
  char *n = malloc(digit + 2);
  if (seq == NULL || n == NULL) {
    printf("Allocation failed.");
    exit(-1);
  }
  char *lb = "(";
  char *rb = ")";
  char *comma = ",";
  seq[0] = '\0';
  for (i = 0; i < nc; i++) {
    strcat(seq, lb);
    for (j = 0; j < nr; j++) {
      sprintf(n, "%d", S[j + i * nr]);
      if (strlen(seq) + strlen(n) + 3 > mem) {
        mem = strlen(seq) + strlen(n) + 3;
        seq = (char *)realloc(seq, mem);
        if (seq == NULL) {
          printf("Allocation failed.");
          exit(-1);
        }
      }
      strcat(seq, n);
      if (j == nr - 1) {
        strcat(seq, rb);
      } else {
        strcat(seq, comma);
      }
    }
  }
  return seq;
}

/********************** Output sequence *****************/

/*
   Show Sequence

   S  = Sequence of BM
   nr = numbers of rows
   nc = numbers of columns
   num = integer as a parameter of BM
         if num=0, [num] is now shown
   cr = CR at the end?

*/

void showSeq(int *S, int nr, long nc, long num, int cr) {
  int i, j;
  for (i = 0; i < nc; i++) {
    printf("(");
    for (j = 0; j < nr - 1; j++) printf("%d,", S[j + i * nr]);
    printf("%d)", S[nr - 1 + i * nr]);
  }
  if (num > 0) {
    printf("[%ld]", num);
  }
  if (cr) {
    printf("\n");
  }
  return;
}

void printM(int *A, int nr, int nc) {
  int r, c;
  for (c = 0; c < nc; c++) {
    printf("(");
    for (r = 0; r < nr - 1; r++) printf("%d,", A[r + c * nr]);
    if (nr > 0) printf("%d)", A[nr - 1 + c * nr]);
  }
}
void printA(int *A, int nr) {
  int r = 0;
  printf("(");
  for (r = 0; r < nr - 1; r++) printf("%d,", A[r]);
  if (nr > 0) printf("%d)", A[nr - 1]);
}

/* Show standard sequence

  nr = numbers of rows
  n  = numbers of columns - 1

*/
void showStd(int nr, long n) {
  int S[nr * (n + 1)], i, j;
  for (i = 0; i <= n; i++) {
    for (j = 0; j < nr; j++) {
      S[j + i * nr] = i;
    }
  }
  showSeq(S, nr, n + 1, 0, 0);
  return;
}

/************** Subroutines for calculation **********/

/* returns index of the element x in P with nr rows
   (it returns -1 when x is not found) */
int find(int *P, int nr, int x) {
  int i;
  for (i = 0; i < nr; i++)
    if (P[i] == x) return i;
  return -1;
}
/* are the all elements equal in nr rows of P */
int areAllEq(int *P, int nr) {
  int i, p0 = P[0];
  for (i = 1; i < nr; i++)
    if (P[i] != p0) return 0;
  return -1;
}
/* argmax returns the index whose element is maximum of P in nr rows */
int argmax(int *P, int nr) {
  int i, m = nr - 1;
  for (i = 0; i < nr; i++)
    if (P[i] > P[m]) m = i;
  return m;
}
/* getParent returns the column number of the parent of the child column c
   in row r in Bashicu matrix S consist of the nr rows
   (it returns -1 when parent is not found) */
int getParent(int *S, int r, int c, int nr) {
  int i;
  for (i = c; i >= 0; i--)
    if (S[r + i * nr] < S[r + c * nr]) return i;
  return -1;
}
/* getParentIB returns the column number of the parent of the child column c in
 * row r in Bashicu matrix S consist of the nr rows with ignored branch model
 * (it returns -1 when parent is not found) */
int getParentIB(int *S, int r, int c, int nr) {
  int x = c;
  int y = r;
  while (x > 0) {
    if (y == 0) { /* 1st row */
      x = x - 1;
    } else { /* not 1st row */
      x = getParentIB(S, y - 1, x, nr);
    }
    if (S[y + x * nr] < S[y + c * nr]) return x;
  }
  return -1; /* no parent */
}

/* getConcestor returns the column number of the concestor of with m rows
   from the child column c in Bashicu matrix S consist of the nr rows
   (it returns -1 when concestor is not found) */
int getConcestor(int *S, int m, int c, int nr) {
  int r, maxr;
  int P[nr];

  for (r = 0; r < m; r++) P[r] = getParent(S, r, c, nr);
  while (find(P, m, -1) == -1) {
    if (areAllEq(P, m)) {
      /* same */
      r = P[0];
      return r;
    } else {
      /* not same */
      maxr = argmax(P, m);
      P[maxr] = getParent(S, maxr, P[maxr], nr);
    }
  }
  return -1;
}

/****************************************************

              Functions for calculation

*****************************************************/

/***************

   getBadSequence - determine the 'bad part'

   This is the first core algorithm of Bashicu Matrix.

   Input variables:
     S: Sequence
     Delta: Just allocate memory
     C: Just allocate memory and initialize at least once
     ver: version of BM
     detail: detail option
     n: numbers of columns - 1
     nr: numbers of rows

   Return:
     bad: numbers of bad part (return variable)
     Delta; numbers to be added when copied (pointer)
     C: C matrix in BM2 (pointer)

*****************************************************/

int getBadSequence(int *S, int *Delta, int *C, int ver, int detail, long n,
                   int nr) {
  int bad = 0; /* Length of bad sequence */
  int found, j, l, m;
  int row = nr - 1;
  long k, p;

  if (S[n * nr] == 0) {
    /* If the first row of S_n is 0, no bad sequence */
    return 0;
  } else {
    if (ver == 100) {
      /***** Version 1 *****/
      /* Calculate m = Uppermost zero at the rightmost sequence */
      m = row + 1; /* row = number of rows + 1 */
      for (j = 0; j <= row; j++) {
        if (S[j + n * nr] == 0) m = j, j = row + 1;
      }
      /* Determine the bad sequence and calculate Delta */
      for (k = 0; k <= n; k++) {
        found = 1;
        for (l = 0; l < m; l++)
          if (S[l + (n - k) * nr] >= S[l + n * nr]) found = 0;
        /* If found, calculate Delta */
        if (found) {
          for (l = 0; l <= row; l++)
            Delta[l] = (l < m - 1) ? S[l + n * nr] - S[l + (n - k) * nr] : 0;
          return k;
          k = n + 1;
        }
      }
    } else if (ver == 200) {
      /***** Version 2 *****/
      /* Clear Delta */
      for (m = 0; m <= row; m++) Delta[m] = 0;
      /* Determine the bad sequence and calculate Delta */
      for (k = 0; k <= n; k++) {     /* k = pivot column */
        for (l = 0; l <= row; l++) { /* l = row */
          if (S[l + (n - k) * nr] < S[l + n * nr] - Delta[l]) {
            if (S[l + 1 + n * nr] == 0 || l == row) {
              l = row;
              bad = k;
              k = n;
            } else {
              Delta[l] = S[l + n * nr] - S[l + (n - k) * nr];
            }
          } else {
            l = row; /* Go to left sequence (k loop) */
          }
        }
      }
      /* Calculate C matrix */
      for (k = 1; k <= bad; k++) {
        /* Find the largest l which satisfies S_(n-bad+l)[0] < S_(n-bad+k)[0] */
        for (l = k; l >= 0; l--) {
          if (S[(n - bad + l) * nr] < S[(n - bad + k) * nr]) {
            for (m = 0; m <= row; m++) {
              C[m + (k + 1) * nr] =
                  (S[m + (n - bad) * nr] < S[m + (n - bad + k) * nr] &&
                   C[m + (l + 1) * nr] == 1)
                      ? 1
                      : 0;
            }
            l = 0;
          }
        }
      }
    } else if (ver == 210) {
      /***** Version 2.1 *****/
      /* Calculate m = Uppermost zero at the rightmost sequence */
      m = row + 1; /* row = number of rows + 1 */
      for (j = 0; j <= row; j++) {
        if (S[j + n * nr] == 0) m = j, j = row + 1;
      }
      /* Determine the bad sequence and calculate Delta */
      for (k = 0; k <= n; k++) {
        /* find and jump to parent of n */
        for (l = 1; n - (k + l) >= 0; l++) {
          if (S[0 + (n - (k + l)) * nr] < S[0 + (n - k) * nr]) {
            k = k + l; /* jump to parent */
            break;
          }
        }
        found = 1;
        for (l = 0; l < m; l++) {
          if (S[l + (n - k) * nr] >= S[l + n * nr]) {
            found = 0;
            break;
          }
        }
        /* If all dimension are small, calculate Delta */
        if (found) {
          for (l = 0; l <= row; l++) {
            Delta[l] = (l < m - 1) ? S[l + n * nr] - S[l + (n - k) * nr] : 0;
          }
          bad = k;
          k = n + 1;
          if (detail) {
            printf("S[bad=%ld]=(", (n - bad));
            for (l = 0; l < row; l++) printf("%d,", S[l + (n - bad) * nr]);
            printf("%d)\n", S[row + (n - bad) * nr]);
          }
        } else {
          k = k - 1;
        }
      }
      return bad;
    } else if (ver == 220) {
      /***** Version 2.2 *****/
      /* Calculate m = Uppermost zero at the rightmost sequence */
      m = row + 1; /* row = number of rows + 1 */
      for (j = 0; j <= row; j++) {
        if (S[j + n * nr] == 0) m = j, j = row + 1;
      }
      /* Determine the bad sequence and calculate Delta */
      l = getConcestor(S, m, n, nr);
      if (l >= 0) {
        bad = n - l;
      } else {
        /* no concestor */
        bad = 0;
      }
      /* If all dimension are small, calculate Delta */
      for (j = 0; j <= row; j++) {
        Delta[j] = (j < m - 1) ? S[j + n * nr] - S[j + (n - bad) * nr] : 0;
      }
    } else if (ver == 300) {
      /***** Version 3 *****/
      /* Clear Delta */
      for (m = 0; m <= row; m++) Delta[m] = 0;
      /* Determine the bad sequence and calculate Delta */
      /* Note that this part is same as version 2 */

      for (k = 0; k <= n; k++) {     /* k = pivot column */
        for (l = 0; l <= row; l++) { /* l = row */
          if (S[l + (n - k) * nr] < S[l + n * nr] - Delta[l]) {
            if (S[l + 1 + n * nr] == 0 || l == row) {
              l = row;
              bad = k;
              k = n;
            } else {
              Delta[l] = S[l + n * nr] - S[l + (n - k) * nr];
            }
          } else {
            l = row; /* Go to left sequence (k loop) */
          }
        }
      }
    } else if (ver == 230) {
      /***** Version 2.3 *****/
      /* Clear Delta */
      for (m = 0; m <= row; m++) Delta[m] = 0;
      /* Determine the bad sequence and calculate Delta (method is same as the
       * one of BM2) */
      for (k = 0; k <= n; k++) {     /* k = pivot column */
        for (l = 0; l <= row; l++) { /* l = row */
          if (S[l + (n - k) * nr] < S[l + n * nr] - Delta[l]) {
            if (S[l + 1 + n * nr] == 0 || l == row) {
              l = row;
              bad = k;
              k = n;
            } else {
              Delta[l] = S[l + n * nr] - S[l + (n - k) * nr];
            }
          } else {
            l = row; /* Go to left sequence (k loop) */
          }
        }
      }
      /* Calculate C matrix */
      for (j = 0; j < nr; j++) {
        for (k = 0; k < bad; k++) {
          p = k;
          while (1) {
            if (p == 0) { /* p reached the bad root */
              C[j + (k + 1) * nr] = 1;
              break;
            } else if (p < 0) { /* p lost the bad root */
              C[j + (k + 1) * nr] = 0;
              break;
            } else { /* p have not reach the bad root */
              p = getParentIB(S, j, p + n - bad, nr) -
                  (n - bad); /* find next parent */
            }
          } /* while find parent */
        }   /* for k */
      }     /* for j */
    }       /* if(ver==230) */
    return bad;
  } /* if(S[n * nr] == 0) */
}

/********************
   copyBadSequence - copy the "bad part"

   This is the second core algorithm of Bashicu Matrix.

   Input variables:
     S: Sequence
     Delta; numbers to be added when copied
     C: C matrix in BM2
     ver: version of BM
     n: numbers of colums - 1
     nn: numbers of columns after copying bad part - 1
     nr: numbers of rows

   Return:
     S: Sequence
     n: numbers of colums (pointer)
        Note that it is NOT numbers of columns - 1.
        In for loop at the main function, n-- is run each time.

***************************/

void copyBadSequence(int *S, int *Delta, int *C, int ver, long *n, long nn,
                     int nr, int bad) {
  int l, m, K, L, M;
  int row = nr - 1;
  if (ver == 100 || ver == 210 || ver == 220) {
    /***** Version 1 *****/
    while (*n < nn) {
      for (l = 0; l <= row; l++)
        S[l + *n * nr] = S[l + (*n - bad) * nr] + Delta[l];
      *n = *n + 1;
    }
  } else if (ver == 200 || ver == 230) {
    /***** Version 2 *****/
    m = 1;
    while (*n < nn) {
      for (l = 0; l <= row; l++) {
        S[l + *n * nr] = S[l + (*n - bad) * nr] + Delta[l] * C[l + m * nr];
      }
      m++;
      *n = *n + 1;
      if (m > bad) m = 1;
    }
  } else if (ver == 300) {
    /***** Version 3 *****/
    K = 1; /* Equivalent to K in original BASIC code */
    while (*n < nn) {
      for (L = 0; L <= row; L++) {
        /* L is equivalent to L in the original BASIC code */
        S[L + *n * nr] = S[L + (*n - bad) * nr];
        /* Add Delta in certain conditions */
        for (m = 0; m <= *n; m++) {
          if (S[(*n - m) * nr] < S[*n * nr]) {
            M = m; /* Now M is determined which is equivalent to BASIC code
                      when condition is met */
            break;
          }
        }
        if (L == 0 || K == 1 ||
            (S[L + (*n - bad - M) * nr] < S[L + (*n - M) * nr] &&
             S[L + (*n - M) * nr] < S[L + *n * nr] + Delta[L])) {
          /* Add Delta in this condition */
          S[L + *n * nr] = S[L + *n * nr] + Delta[L];
        }
        /* Finished adding Delta */
      }
      K++;
      *n = *n + 1;
      if (K > bad) K = 1;
    }
  }
}

/********************

   oneStep - process calculation for one step

   Input variables:
     S: Sequence
     nc: numbers of columns
     nr: numbers of rows
     num: integer parameter of BM
     ver: version of BM
     opt: calculation option

   Return:
     S: Sequence (pointer)
     nc: numbers of columns (pointer)

***************************/

void oneStep(int *S, long *nc, int nr, long num, int ver, int opt) {
  int bad, m;
  int row = nr - 1;
  long nn;
  long n = *nc - 1;
  if (opt == 2) num++;
  if (opt == 3) num *= num;
  if (opt == 4 && S[n * nr] == 0) num++;
  int s = num * (*nc + 1);
  int Delta[nr], C[nr * s];
  for (m = 0; m <= row; m++) {
    C[m + nr] = 1;
  }
  bad = getBadSequence(S, Delta, C, ver, 0, n, nr);
  nn = (opt == 4) ? n + bad * (num - 1) : n + bad * num;
  copyBadSequence(S, Delta, C, ver, &n, nn, nr, bad);
  *nc = n;
  return;
}

/****************************
  Compare 2 sequences S and S2

  returns 1 if S > S2
          2 if S = S2
          3 if S < S2

  error when not determined
******************************/

int cmpSeq(int *S, long nc, int *S2, long nc2, int nr, int ver) {
  int i, j, nc3, found = 0;
  long n;
  int Delta[nr], C[nr * (nc + nc2) * 2];
  for (i = 0; i < nr; i++) {
    C[i + nr] = 1;
  }
  if (nc < nc2) {
    nc3 = nc;
  } else {
    nc3 = nc2;
  }
  for (i = 0; i < nc3; i++) {
    for (j = 0; j < nr; j++) {
      if (S[j + i * nr] != S2[j + i * nr]) {
        found = 1;
      }
    }
    if (found) {
      break;
    }
  }
  if (found == 0) {
    if (nc == nc2) {
      /* printf("%s = %s\n",getSeq(S,nr,nc),getSeq(S2,nr,nc2)); */
      return 2;
    }
    if (nc < nc2) {
      /* printf("%s < %s\n",getSeq(S,nr,nc),getSeq(S2,nr,nc2)); */
      return 3;
    }
    return 1;
  }
  int bad, S3[nc * nr];
  for (j = 0; j < nc * nr; j++) {
    S3[j] = S[j];
  }
  while (1) {
    bad = getBadSequence(S3, Delta, C, ver, 0, i, nr);
    if (bad == 0) {
      return 3;
    }
    n = i;
    copyBadSequence(S3, Delta, C, ver, &n, i + 1, nr, bad);
    found = 0;
    for (j = 0; j < nr; j++) {
      if (S2[j + i * nr] != S3[j + i * nr]) {
        found = 1;
      }
    }
    if (found == 0) {
      /* printf("%s > %s\n",getSeq(S,nr,nc),getSeq(S2,nr,nc2)); */
      return 1;
    }
  }
}

/****************************

   chkStd - check standard format

   S: Sequence
   nc: numbers of columns
   nr: numbers of rows
   ver: version
   detail: show detail

   return 0 if Standard
          1 if not

*****************************/

int chkStd(int *S, long nc, int nr, int ver, int detail) {
  int i, j, row = 0;

  /* check real numbers of row */
  for (i = 0; i < nc; i++) {
    if (row + 1 == nr) {
      break;
    }
    for (j = row + 1; j < nr; j++) {
      if (S[j + i * nr] > 0) {
        row = j;
      }
    }
  }

  /* Find smaller column p */
  int p = -1;
  for (i = 0; i < nc; i++) {
    for (j = 0; j <= row; j++) {
      if (S[j + i * nr] > i) {
        if (detail) {
          showSeq(S, nr, nc, 0, 0);
          printf(" is not standard because ");
          if (i == 0) {
            printf("not starting from ");
            showStd(nr, 0);
            printf("\n");
          } else {
            showStd(nr, i);
            printf(" does not decrease to the sequence.\n");
          }
        }
        return 1;
      }
      if (S[j + i * nr] < i) {
        p = i;
        j = row + 1;
        i = nc;
      }
    }
  }
  if (p < 0) {
    if (detail) {
      showSeq(S, nr, nc, 0, 0);
      printf(" is standard.\n");
    }
    return 0;
  }

  if (detail) {
    printf("Checking if ");
    showSeq(S, nr, nc, 0, 0);
    printf(" is standard.\nDecreasing sequence from ");
    showStd(row + 2, 1);
    printf(" follows.\n");
  }

  /* Make a sequence above S (SA) */
  int SA[(row + 1) * nc * 2], Delta[row + 1], C[(row + 1) * nc * 2], bad, num,
      larger, smaller;
  long nn, p2;
  for (i = 0; i < row + 1; i++) {
    C[i + row + 1] = 1;
  }

  for (i = 0; i <= p; i++) {
    for (j = 0; j < row + 1; j++) {
      SA[j + i * (row + 1)] = i;
    }
  }

  /* Check decreasing sequence of SA */
  while (1) {
    if (detail) {
      showSeq(SA, row + 1, p + 1, 0, 1);
    }
    bad = getBadSequence(SA, Delta, C, ver, 0, p, row + 1);
    if (bad == 0) {
      if (detail) {
        printf("Not standard because it goes directly to ");
        showSeq(SA, row + 1, p, 0, 1);
      }
      return 1;
    }
    num = (nc + 1 - p) / bad + 1;
    nn = p + bad * num;
    p2 = p;
    copyBadSequence(SA, Delta, C, ver, &p2, nn, row + 1, bad);
    p2 = -1;
    for (i = p; i < nc; i++) {
      smaller = 0;
      larger = 0;
      for (j = 0; j < row + 1; j++) {
        if (S[j + i * nr] > SA[j + i * (row + 1)]) {
          larger = 1;
        }
        if (S[j + i * nr] < SA[j + i * (row + 1)]) {
          smaller = 1;
        }
      }
      if (larger == 1 && smaller == 0) {
        if (detail) {
          printf("Not standard because it is ");
          showSeq(SA, row + 1, i + 1, 0, 0);
          printf("...\n");
        }
        return 1;
      }
      if (larger == 1 || smaller == 1) {
        p2 = i;
        i = nc;
      }
    }
    if (p2 == -1) {
      if (detail) {
        showSeq(SA, row + 1, nc, 0, 1);
        printf("Standard.\n");
      }
      return 0;
    }
    p = p2;
  }
}

/************************
  getOrd - Ordinal analysis

  S: Sequence
  nc: numbers of columns
  nr: numbers of rows
  ver: version
  form: output format (reserved)
        100 to return "e" when it is epsilon number

  Return the ordinal

*************************/

char *getOrd(int *S, long nc, int nr, int ver, int form) {
  int i, j, k, found;
  char *ordinal = malloc(nc * 10 + 12);
  if (ordinal == NULL) {
    printf("Allocation failed.");
    exit(-1);
  }
  char *plus = "+";
  char *omega = "w";
  char *omegahut = "w^";
  char *epsilon = "e_";
  char *psi = "p";
  char *lp = "(";
  char *rp = ")";
  char *zero = "0";
  char *twotimes = "*2";

  /* Check if standard */
  int std = chkStd(S, nc, nr, ver, 0);
  if (std == 1) {
    return "Not standard";
  }

  /* (0,0) = 1 */
  if (nc == 1) {
    return ("1");
  }

  /*
    Theorems 1 and 2 of Deedlit11 from
  https://googology.wikia.com/wiki/Talk:Pair_sequence_number

  Theorem 1. (0,0) basically separates the sequence into summands, so for
  example

  |(0,0) A (0,0) B (0,0) C| = |(0,0) A| + |(0,0) B| + |(0,0) C|

  */

  for (i = nc - 1; i > 0; i--) {
    found = 0;
    for (j = 0; j < nr; j++) {
      if (S[j + i * nr] > 0) {
        found = 1;
      }
    }
    if (found == 0) {
      if (i == nc - 1) { /* successor ordinal */
        for (i = nc - 1; i >= 0; i--) {
          found = 0;
          for (j = 0; j < nr; j++) {
            if (S[j + i * nr] > 0) {
              found = 1;
            }
          }
          if (found == 1) {
            break;
          }
        }
        i++;
        char ord2[4];
        sprintf(ord2, "%ld", nc - i);
        if (i == 0) {
          strcpy(ordinal, ord2);
          return ordinal;
        }
        char *ord1 = getOrd(S, i, nr, ver, form);
        strcpy(ordinal, ord1);
        strcat(ordinal, plus);
        strcat(ordinal, ord2);
        return ordinal;
      }
      char *ord1 = getOrd(S, i, nr, ver, form);
      int S2[(nc - 1) * nr];
      for (j = 0; j < (nc - i) * nr; j++) {
        S2[j] = S[j + i * nr];
      }
      char *ord2 = getOrd(S2, nc - i, nr, ver, form);
      strcpy(ordinal, ord1);
      strcat(ordinal, plus);
      strcat(ordinal, ord2);
      return ordinal;
    }
  }

  /*
  Theorem 2. In a part of the sequence with no (0,0)'s, the (1,0)'s satisfy the
  following:

  |(0,0) A (1,0) B| = |(0,0)A| * w^|[(1,0) B - (1,0)]|
  */

  for (i = 1; i < nc; i++) {
    found = 0;
    if (S[i * nr] != 1) {
      found = 1;
    }
    for (j = 1; j < nr; j++) {
      if (S[j + i * nr] > 0) {
        found = 1;
      }
    }
    if (found == 0) {
      char *ord1 = getOrd(S, i, nr, ver, form);
      int S2[(nc - 1) * nr];
      if (i == 1) {
        /* 1w^a = w^a */
        ord1 = "";
      }
      if (i == nc - 1) {
        /* aw^1 = aw */
        if (i == 1) {
          /* 1w^a = w^a */
          strcpy(ordinal, omega);
        } else {
          strcpy(ordinal, lp);
          strcat(ordinal, ord1);
          strcat(ordinal, rp);
          strcat(ordinal, omega);
        }
        return ordinal;
      }
      for (j = 0; j < nc - i; j++) {
        /* assertion because it is standard format */
        assert(S[(i + j) * nr] > 0);
        S2[j * nr] = S[(i + j) * nr] - 1;
        for (k = 1; k < nr; k++) {
          S2[k + j * nr] = S[k + (i + j) * nr];
        }
      }
      char *ord2 = getOrd(S2, nc - i, nr, ver, form);
      /* Check if a is epsilon number */
      if (strcmp(getOrd(S, i, nr, 100, 100), "e") == 0) {
        /* a is epsilon number, so aw^b = w^(ab) */
        if (strcmp(ord1, ord2) == 0) {
          /* if a=b, return w^w^(a2)
            because aw^a = (w^a)^2 = w^(aa) = w^((w^a)2) = w^w^(a2)
          */
          strcpy(ordinal, omegahut);
          strcat(ordinal, omegahut);
          strcat(ordinal, lp);
          strcat(ordinal, ord1);
          strcat(ordinal, twotimes);
          strcat(ordinal, rp);
          return ordinal;
        }
        if (ord2[0] == 'w' && ord2[1] == '^') {
          /* If b=w^c, return w^w^(a+c)
             First compare a and b
             if a<b. w^a<w^c and a<c. therefore return w^w^c = w^b
           */
          j = cmpSeq(S, i, S2, nc - i, nr, ver);
          if (j == 3) {
            strcat(ordinal, omegahut);
            strcat(ordinal, ord2);
            return ordinal;
          }

          /* Return w^w^(a+c) */
          strcpy(ordinal, omegahut);
          strcat(ordinal, omegahut);
          strcat(ordinal, lp);
          strcat(ordinal, ord1);
          strcat(ordinal, plus);
          strcat(ordinal, ord2 + 2);
          strcat(ordinal, rp);
          return ordinal;
        }
        if (strcmp(getOrd(S2, nc - i, nr, 100, 100), "e") == 0) {
          /* If b is epsilon number, return w^w^(a+b) */
          strcpy(ordinal, omegahut);
          strcat(ordinal, omegahut);
          strcat(ordinal, lp);
          strcat(ordinal, ord1);
          strcat(ordinal, plus);
          strcat(ordinal, ord2);
          strcat(ordinal, rp);
          return ordinal;
        }
        /* Return w^(ab) */
        strcpy(ordinal, omegahut);
        strcat(ordinal, lp);
        strcat(ordinal, lp);
        strcat(ordinal, ord1);
        strcat(ordinal, rp);
        if (strchr(ord2, '+') == NULL && strchr(ord2, '_') == NULL) {
          strcat(ordinal, ord2);
        } else {
          strcat(ordinal, lp);
          strcat(ordinal, ord2);
          strcat(ordinal, rp);
        }
        strcat(ordinal, rp);
        return ordinal;
      }

      /* Now return aw^b */
      if (i == 1) {
        /* 1w^a = w^a */
        strcpy(ordinal, omegahut);
      } else {
        strcpy(ordinal, lp);
        strcat(ordinal, ord1);
        strcat(ordinal, rp);
        strcat(ordinal, omegahut);
      }
      if (strchr(ord2, '+') == NULL && strchr(ord2, '_') == NULL) {
        strcat(ordinal, ord2);
      } else {
        strcat(ordinal, lp);
        strcat(ordinal, ord2);
        strcat(ordinal, rp);
      }
      return ordinal;
    }
  }

  /*
     For ordials below epsilon_0, all the ordinals are determined
     with the algorighm up to here.

     Moreover as we deleted all (0,0) (except for the first column) and (1,0),
     it is epsilon number.
  */

  /*
     When form = 100, just return "e" to indicate epsilon number
  */

  if (form == 100) {
    return "e";
  }

  /* Analysis above pair sequence is only for BM2 or 2.3 */

  if (ver != 200 && ver != 230) {
    ordinal = getSeq(S, nr, nc);
    return ordinal;
  }

  /* Second column is (1,1,x,...) Because it is standard
     (0,0)(1,0)... is not standard.
   */

  assert(S[nr] == 1);
  assert(S[1 + nr] == 1);

  if (nr == 2 || S[2 + nr] == 0) { /* Pair sequence */

    /* epsilon_0 */
    if (nc == 2) {
      strcpy(ordinal, epsilon);
      strcat(ordinal, zero);
      return ordinal;
    }
    if (S[2 * nr] == 2 && S[1 + 2 * nr] > 0) {
      /* Start from (0,0)(1,1)(2,2) or (0,0)(1,1)(2,1)

      Using koteitan's algorithm
      http://www.ukaibutton.com/googology/p2b.html

      */
      strcpy(ordinal, psi);
      strcat(ordinal, zero);
      strcat(ordinal, lp);
      char buf[5];
      for (i = 1; i < nc; i++) {
        if (S[i * nr] > S[(i - 1) * nr]) {
          strcat(ordinal, psi);
          snprintf(buf, 5, "%d", S[1 + i * nr]);
          strcat(ordinal, buf);
          strcat(ordinal, lp);
        } else {
          strcat(ordinal, zero);
          for (j = S[i * nr]; j < S[(i - 1) * nr] + 1; j++) {
            strcat(ordinal, rp);
          }
          strcat(ordinal, plus);
          strcat(ordinal, psi);
          snprintf(buf, 5, "%d", S[1 + i * nr]);
          strcat(ordinal, buf);
          strcat(ordinal, lp);
        }
      }
      strcat(ordinal, zero);
      for (i = 0; i < S[(nc - 1) * nr] + 1; i++) {
        strcat(ordinal, rp);
      }
      return ordinal;
    }
    /*

    Now the ordinals from epsilon_0 to below zeta_0
    (0,0)(1,1)A, where A starts from (a,0) (a>1) or (1,1)
    This is epsilon numbers, epsilon_alpha.
    We find a sequence which expresses alpha, and put it in S2.

    */
    int nc2, S2[nc * 2];
    if (S[1 + 2 * nr] == 0) {
      /* (0,0)(1,1)(2,0) */
      assert(S[2 * nr] == 2);
      if (nc > 3 && S[3 * nr] == 3 && S[1 + 3 * nr] == 1) {
        /* Begin with (0,0)(1,1)(2,0)(3,1)
           above epsilon_epsilon_0
           alpha >= epsilon_0
         */

        /* Alpha begins with (0,0)(1,1) */
        S2[0] = 0;
        S2[1] = 0;
        S2[2] = 1;
        S2[3] = 1;

        if (nc == 4) {
          /* alpha = epsilon_0 = (0,0)(1,1) */
          nc2 = 2;
        } else {
          /* Now S2 from 3rd column is determined in 2 steps.
            (1) Cut left nodes
            (3) If the column is (1,1), make it (0,0)
            (2) Otherwise make (a,0) to (a-offset,0)

            First determine cut and offset

            Check the 5th cell which follows (3,1)

            5th call = (1,1)
            (0,0)(1,1)(2,0)(3,1)(1,1) = e_(e_0+1)
                      (0,0)(1,1)(0,0) = e_0+1

            5th cell = (a,0) a<3: cut = 2, offset=1

            (0,0)(1,1)(2,0)(3,1)(2,0) = e_(e_0 w)
                      (0,0)(1,1)(1,0) = e_0 w

            5th cell = (3,0): cut = 0, offset = 1

            (0,0)(1,1)(2,0)(3,1)(3,0) = e_(e_0^w)
            (0,0)(1,1)(1,0)(2,1)(2,0) = e_0^w

            5th cell = (a,b) a>2, except (3,0):
                       cut = 2, offset=1

            (0,0)(1,1)(2,0)(3,1)(3,1) = e_(e_1)
                      (0,0)(1,1)(1,1) = e_1
            (0,0)(1,1)(2,0)(3,1)(4,0) = e_(e_w)
                      (0,0)(1,1)(2,0) = e_w
            (0,0)(1,1)(2,0)(3,1)(4,0)(5,1) = e_(e_(e_0))
                      (0,0)(1,1)(2,0)(3,1) = e_(e_0)
          */
          int cut = 2, offset = 1;
          if (S[4 * nr] > 2) {
            offset = 2;
          }
          if (S[4 * nr] == 3 && S[1 + 4 * nr] == 0) {
            cut = 0, offset = 1;
          }

          /* Then do the cut and offset */
          nc2 = nc - cut;
          for (i = 2; i < nc2; i++) {
            if (S[(i + cut) * nr] == 1 && S[1 + (i + cut) * nr] == 1) {
              /* (1,1) -> (0,0) */
              S2[i * 2] = 0;
              S2[1 + i * 2] = 0;
            } else {
              /* (a,b) to (a-offset,b) */
              S2[i * 2] = S[(i + cut) * nr] - offset;
              S2[1 + i * 2] = S[1 + (i + cut) * nr];
              /* if it has (0,0)(1,0)(2,1) pattern, change to (0,0)(1,1)
                 to make it standard form
              (0,0)(1,1)(2,0)(3,1)(1,1)(2,0)(3,1)(1,1)(2,0)(3,1) = e_(e_0*3) */
              if (i > 1 && S2[i * 2 - 4] == 0 && S2[i * 2 - 3] == 0 &&
                  S2[i * 2 - 2] == 1 && S2[i * 2 - 1] == 0 && S2[i * 2] == 2 &&
                  S2[i * 2 + 1] == 1) {
                S2[i * 2 - 1] = 1;
                i--;
                cut++;
                nc2--;
              }
            }
          }
        }
      } else {
        /* (0,0)(1,1)(2,0)A where A doesn't start with (3,1)
           In this pattern, S2 starts from (0,0) and
           S2 from the 2nd column is determined with
           cut = 1, offset = 1 algorithm
         */
        nc2 = nc - 1;
        /* (0,0) first */
        S2[0] = 0;
        S2[1] = 0;
        for (i = 1; i < nc2; i++) {
          if (S[1 + (i + 1) * nr] == 1) {
            /* (1,1) -> (0,0) */
            assert(S[(i + 1) * nr] == 1);
            S2[i * 2] = 0;
            S2[i * 2 + 1] = 0;
          }
          /* (a,0) to (a-1,0) */
          S2[i * 2] = S[(i + 1) * nr] - 1;
          S2[i * 2 + 1] = 0;
        }
      }
    } else {
      /* (0,0)(1,1)(1,1)A
         This pattern is just cut=2 and offset = 1
       */
      assert(S[2 * nr] == 1);
      assert(S[1 + 2 * nr] == 1);
      nc2 = nc - 2;
      for (i = 0; i < nc2; i++) {
        if (S[1 + (i + 2) * nr] == 1) {
          /* (1,1) -> (0,0) */
          assert(S[(i + 1) * nr] == 1);
          S2[i * 2] = 0;
          S2[i * 2 + 1] = 0;
        }
        /* (a,0) to (a-1,0) */
        S2[i * 2] = S[(i + 2) * nr] - 1;
        S2[i * 2 + 1] = 0;
      }
    }
    /* Now we determined S2 for all patterns.
       Convert S2 to ordinal alpha, and returns epsilon_alpha
    */
    char *ord1 = getOrd(S2, nc2, 2, ver, form);
    strcat(ordinal, epsilon);
    if (strlen(ord1) == 1) {
      strcat(ordinal, ord1);
    } else {
      strcat(ordinal, lp);
      strcat(ordinal, ord1);
      strcat(ordinal, rp);
    }
    return ordinal;
  }

  /* Trio sequence
   */

  assert(nr > 2);
  assert(S[1 + nr] == 1);

  if (nr == 3 || S[3 + nr] == 0) { /* Trio sequence */
    /* p0(pw(0)) */
    if (nc == 2) {
      return "p0(pw(0))";
    }
  }

  /* Now going beyond the current analysis level and just
     return the sequence as the ordinal */

  ordinal = getSeq(S, nr, nc);
  return ordinal;
}

/* showDetail shows detailed process */
void showDetail(int *S, int *Delta, int *C, int ver, long n, int nr, long num,
                int bad) {
  int l, m;
  int form = 0;
  char *seq = getSeq(S, nr, n + 1);
  int std = chkStd(S, n + 1, nr, ver, 0);
  if (std == 0) {
    char *ordinal = getOrd(S, n + 1, nr, ver, form);
    if (strcmp(ordinal, seq)) {
      printf("Ord = %s\n", ordinal);
    }
  }
  printf("G = ");
  for (l = 0; l < n; l++) {
    if (l == n - bad) {
      if (l == 0) printf("empty");
      printf("\nB = ");
    }
    printf("(");
    for (m = 0; m < nr - 1; m++) printf("%d,", S[m + l * nr]);
    printf("%d)", S[nr - 1 + l * nr]);
  }
  printf("\nDelta = (");
  for (l = 0; l < nr - 1; l++) printf("%d,", Delta[l]);
  printf("%d)\n", Delta[nr - 1]);
  /* Show C matrix */
  if (ver == 200 || ver == 230) {
    printf("C = ");
    for (l = 1; l <= bad; l++) {
      printf("(");
      for (m = 0; m < nr - 1; m++) printf("%d,", C[m + l * nr]);
      printf("%d)", C[nr - 1 + l * nr]);
    }
    printf("\n");
  }
  /* Show f(n) */
  printf("f(n) = %ld\n", num);
  return;
}

/************************* Test codes ************************/

/*
   testOne - Test One step

*/

void testOne(char *bm, char *expected, int nr, int ver, long num, int opt) {
  int i, j, len, mem;
  len = strlen(bm);
  mem = len + strlen(expected);
  int S[mem];
  long nc = len / nr;
  for (i = 0; i < len; i++) {
    S[i] = bm[i] - '0';
  }
  oneStep(S, &nc, nr, num, ver, opt);
  char *actual = malloc(nc * (nr + 1)), *digit = malloc(3);
  if (actual == NULL || digit == NULL) {
    printf("Allocation failed.");
    exit(-1);
  }
  for (i = 0; i < nc * nr; i++) {
    sprintf(digit, "%d", S[i]);
    if (i == 0) {
      strcpy(actual, digit);
    } else {
      strcat(actual, digit);
    }
  }
  if (strcmp(expected, actual) == 0) {
    free(actual);
    free(digit);
    return;
  }
  printf("Test failed with ");
  for (i = 0; i < len / nr; i++) {
    printf("(");
    for (j = 0; j < nr - 1; j++) printf("%d,", bm[j + i * nr] - '0');
    printf("%d)", bm[nr - 1 + i * nr] - '0');
  }
  printf("[%ld]\n", num);
  printf("ver = %d, opt = %d\n", ver, opt);
  printf("Input    = %s\nExpected = %s\nActual   = %s\n", bm, expected, actual);
  free(actual);
  free(digit);
  exit(-1);
}

/*
  testStd - test standard format

*/

void testStd(char *bm, int expected, int nr, int ver) {
  int i, j, len, actual;
  len = strlen(bm);
  int S[len];
  long nc = len / nr;
  for (i = 0; i < len; i++) {
    S[i] = bm[i] - '0';
  }
  actual = chkStd(S, nc, nr, ver, 0);
  if (expected == actual) {
    return;
  }
  printf("Test failed with ");
  for (i = 0; i < len / nr; i++) {
    printf("(");
    for (j = 0; j < nr - 1; j++) printf("%d,", bm[j + i * nr] - '0');
    printf("%d)", bm[nr - 1 + i * nr] - '0');
  }
  printf("\nver = %d\n", ver);
  printf("Expected = %d\nActual   = %d\n", expected, actual);
  exit(-1);
}

/*
  testOrd - test ordinal analysis

*/

void testOrd(char *bm, char *expected, int nr, int ver, int detail) {
  int i, len;
  int form = 0;
  len = strlen(bm);
  int S[len];
  char *ordinal = "";
  long nc = len / nr;
  for (i = 0; i < len; i++) {
    S[i] = bm[i] - '0';
  }
  ordinal = getOrd(S, nc, nr, ver, form);
  if (strcmp(expected, ordinal) == 0) {
    if (detail) {
      /* printf("*"); *** for posting on wikia */
      showSeq(S, nr, nc, 0, 0);
      printf(" = %s\n", ordinal);
    }
    return;
  }
  printf("Test failed with ");
  showSeq(S, nr, nc, 0, 1);
  printf("ver = %d\n", ver);
  printf("Expected = %s\nActual   = %s\n", expected, ordinal);
  exit(-1);
}

/*
  Test cmpSeq
*/

void testCmpseq(char *bm, char *bm2, int expected, int nr, int ver) {
  testStd(bm, 0, nr, ver);
  testStd(bm2, 0, nr, ver);
  int S[100], S2[100], i, len;
  len = strlen(bm);
  long nc = len / nr;
  for (i = 0; i < len; i++) {
    S[i] = bm[i] - '0';
  }
  len = strlen(bm2);
  long nc2 = len / nr;
  for (i = 0; i < len; i++) {
    S2[i] = bm2[i] - '0';
  }
  i = cmpSeq(S, nc, S2, nc2, nr, ver);
  if (i == expected) {
    return;
  }
  printf("Test failed with\n");
  showSeq(S, nr, nc, 0, 1);
  showSeq(S2, nr, nc2, 0, 1);
  printf("ver = %d\n", ver);
  printf("Expected = %d\nActual   = %d\n", expected, i);
  exit(-1);
}

/*********************

  testall - Test everything

  Run this test by

    basmat -T

  Input sequence is given by onecharacter-one number basis, and numbers
  of rows are determined by the parameter "nr".

**********************/

void testAll(detail) {
  /* Test CompSeq function */
  testCmpseq("001122", "0011", 1, 2, 100);
  testCmpseq("001122", "001122", 2, 2, 100);
  testCmpseq("0011", "001122", 3, 2, 100);
  testCmpseq("001121", "001122", 3, 2, 100);
  testCmpseq("00112230", "00112221", 1, 2, 100);
  testCmpseq("00112221", "00112230", 3, 2, 100);

  /* Some basic calculations */
  /* testOne(input, expected, nr, ver, num, opt) */
  testOne("0122", "0121212", 1, 100, 2, 1);
  testOne("0122", "012121212", 1, 200, 2, 2);
  testOne("0122", "01212121212", 1, 300, 2, 3);
  testOne("0122", "01212", 1, 210, 2, 4);
  testOne("0123", "012222222222", 1, 100, 3, 3);
  testOne("0011", "00102030", 2, 220, 3, 1);
  testOne("00001100", "0000100020003000", 4, 220, 3, 1);
  testOne("001100", "0011", 2, 230, 2, 4);
  testOne("001101", "0011", 2, 200, 2, 3);
  testOne("001110", "001100110011", 2, 200, 2, 1);
  testOne("000111", "000110220330", 3, 100, 3, 1);
  testOne("000011112222", "0000111122213331", 4, 100, 2, 1);

  /* (0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,1,0)(3,1,0)[1] */
  testOne("000111200110210310", "000111200110210300210310", 3, 100, 1, 1);
  testOne("000111200110210310", "000111200110210300411500410510", 3, 200, 1, 1);
  testOne("000111200110210310", "000111200110210300411500410510", 3, 210, 1, 1);
  testOne("000111200110210310", "000111200110210", 3, 220, 1, 1);
  testOne("000111200110210310", "000111200110210300411500410510", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,0,0)(1,1,1)[1] */
  testOne("000111200111", "000111200110221310", 3, 100, 1, 1);
  testOne("000111200111", "000111200110221300", 3, 200, 1, 1);
  testOne("000111200111", "000111200110221310", 3, 210, 1, 1);
  testOne("000111200111", "000111200", 3, 220, 1, 1);
  testOne("000111200111", "000111200110221300", 3, 230, 1, 1);
  testOne("000111200111", "000111200110221300", 3, 300, 1, 1);

  /* (0,0,0)(1,0,0)(2,0,0)(2,1,0)(3,1,1)[1] */
  testOne("000100200210311", "000100200210310320", 3, 100, 1, 1);
  testOne("000100200210311", "000100200210310400420", 3, 200, 1, 1);
  testOne("000100200210311", "000100200210310410420", 3, 210, 1, 1);
  testOne("000100200210311", "000100200210", 3, 220, 1, 1);
  testOne("000100200210311", "000100200210310400420", 3, 230, 1, 1);
  testOne("000100200210311", "000100200210310400420", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,1,0)(1,1,1)[1] */
  testOne("000111210111", "000111210110221320", 3, 200, 1, 1);
  testOne("000111210111", "000111210110221320", 3, 210, 1, 1);
  testOne("000111210111", "000111210", 3, 220, 1, 1);
  testOne("000111210111", "000111210110221320", 3, 230, 1, 1);
  testOne("000111210111", "000111210110221310", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,1,1)(1,1,1)[1] */
  testOne("000111211111", "000111211110221321", 3, 200, 1, 1);
  testOne("000111211111", "000111211110221321", 3, 210, 1, 1);
  testOne("000111211111", "000111211110221321", 3, 220, 1, 1);
  testOne("000111211111", "000111211110221321", 3, 230, 1, 1);
  testOne("000111211111", "000111211110221311", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,2,2)(3,1,0)(2,2,1)[1] */
  testOne("000111222310221", "000111222310220331442530", 3, 200, 1, 1);
  testOne("000111222310221", "000111222310220331442530", 3, 210, 1, 1);
  testOne("000111222310221", "000111222310", 3, 220, 1, 1);
  testOne("000111222310221", "000111222310220331442530", 3, 230, 1, 1);
  testOne("000111222310221", "000111222310220331442510", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,2,2)(3,1,0)(2,2,2)[1] */
  testOne("000111222310222", "000111222310221332420", 3, 100, 1, 1);
  testOne("000111222310222", "000111222310221332410", 3, 200, 1, 1);
  testOne("000111222310222", "000111222310221332420", 3, 210, 1, 1);
  testOne("000111222310222", "000111222310", 3, 220, 1, 1);
  testOne("000111222310222", "000111222310221332410", 3, 230, 1, 1);
  testOne("000111222310222", "000111222310221332410", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,2,2)(3,1,1)(2,2,2)[1] */
  testOne("000111222311222", "000111222311221332421", 3, 100, 1, 1);
  testOne("000111222311222", "000111222311221332411", 3, 200, 1, 1);
  testOne("000111222311222", "000111222311221332421", 3, 210, 1, 1);
  testOne("000111222311222", "000111222311220331442531", 3, 220, 1, 1);
  testOne("000111222310222", "000111222310221332410", 3, 230, 1, 1);
  testOne("000111222311222", "000111222311221332411", 3, 300, 1, 1);

  /* (0,0,0,0)(1,1,1,1)(2,2,1,1)(3,3,1,1)(4,2,0,0)(5,1,1,1)(6,2,1,1)(7,3,1,1) */
  testOne("00001111221133114200511162117311",
          "000011112211331142005111621173108421952110621115001241113511", 4,
          200, 1, 1);
  testOne("00001111221133114200511162117311",
          "0000111122113311420051116211731082219321", 4, 210, 1, 1);
  testOne("00001111221133114200511162117311", "0000111122113311420051116211", 4,
          220, 1, 1);
  testOne("00001111221133114200511162117311",
          "000011112211331142005111621173108421952110621115001242113521", 4,
          230, 1, 1);

  /* Test standard format */
  /* testStd(bm, expected, nr, ver) */
  testStd("012", 0, 1, 100);
  testStd("100100100", 1, 3, 100);
  testStd("0011223544", 1, 2, 100);
  testStd("001111212233", 1, 2, 100);
  testStd("001111102233", 1, 2, 100);
  testStd("001111102100", 0, 2, 100);
  testStd("000111211111", 0, 3, 300);
  testStd("000100200210311", 1, 3, 200);
  testStd("000111110221110211", 1, 3, 200);
  testStd("000111211110221311", 1, 3, 200);
  testStd("000111211110221311", 0, 3, 300);
  testStd("000111221222", 1, 3, 200);
  testStd("000111222123", 1, 3, 200);
  testStd("000111222320222", 1, 3, 220);
  testStd("00001111221133114200511162117311", 0, 4, 230);

  /* Test ordinal analysis */
  testOrd("001", "Not standard", 1, 100, detail);
  testOrd("0", "1", 1, 100, detail);
  testOrd("000000", "6", 1, 100, detail);
  testOrd("0100", "w+2", 1, 100, detail);
  testOrd("010101", "w+w+w", 1, 100, detail); /* = w3 not calculated */
  testOrd("0111", "w^3", 1, 100, detail);
  testOrd("012", "w^w", 1, 100, detail);
  testOrd("0123", "w^w^w", 1, 100, detail);
  testOrd("012323", "w^(w^(w+w))", 1, 100, detail);
  testOrd("01233000", "w^w^w^2+3", 1, 100, detail);
  testOrd("0123423", "w^(w^(w^w+w))", 1, 100, detail);
  testOrd("012345621233012", "w^(w^(w^w^w^w+1)+w^w^2)+w^w", 1, 100, detail);
  /* pair sequence only for BM2 and 2.3 */
  testOrd("0011", "(0,0)(1,1)", 2, 100, 0);
  testOrd("0011", "e_0", 2, 200, 0);
  testOrd("0011", "(0,0)(1,1)", 2, 210, 0);
  testOrd("0011", "(0,0)(1,1)", 2, 220, 0);
  testOrd("0011", "e_0", 2, 230, 0);
  testOrd("0011", "(0,0)(1,1)", 2, 300, 0);
  testOrd("00111000", "(e_0)w+1", 2, 200, detail);
  testOrd("00111010", "w^((e_0)2)", 2, 230, detail);
  testOrd("00111020", "w^((e_0)w)", 2, 200, detail);
  testOrd("00111021", "w^w^(e_0*2)", 2, 200, detail);
  testOrd("0011102120", "w^((e_0)((e_0)w))", 2, 200, detail); /* = w^((e_0)w) */
  testOrd("001110212031", "w^w^w^(e_0*2)", 2, 200, detail);
  testOrd("001111", "e_1", 2, 200, detail);
  testOrd("001111102121203131", "w^w^w^(e_1*2)", 2, 200, detail);
  testOrd("0011111111", "e_3", 2, 200, detail);
  testOrd("001120", "e_w", 2, 200, detail);
  testOrd("001120102121", "w^w^(e_w+e_1)", 2, 200, detail);
  testOrd("001120102130203140", "w^w^w^(e_w*2)", 2, 200, detail);
  testOrd("00112011", "e_(w+1)", 2, 200, detail);
  testOrd("00112020", "e_(w^2)", 2, 200, detail);
  testOrd("00112030", "e_(w^w)", 2, 200, detail);
  testOrd("001120301120", "e_(w^w+w)", 2, 200, detail);
  testOrd("0011203040", "e_(w^w^w)", 2, 200, detail);
  testOrd("00112031", "e_(e_0)", 2, 200, detail);
  testOrd("0011203111", "e_(e_0+1)", 2, 200, detail);
  testOrd("00112031112031", "e_(e_0+e_0)", 2, 200, detail);
  testOrd("00112031112031112031", "e_(e_0+e_0+e_0)", 2, 200, detail);
  testOrd("0011203120", "e_((e_0)w)", 2, 200, detail);
  testOrd("00112031201111", "e_((e_0)w+2)", 2, 200, detail);
  testOrd("001120312020", "e_(w^((e_0)2))", 2, 200, detail);
  testOrd("001120312030", "e_(w^((e_0)w))", 2, 200, detail);
  testOrd("001120312031", "e_(w^w^(e_0*2))", 2, 200, detail);
  testOrd("0011203130", "e_(w^((e_0)((e_0)w)))", 2, 200, detail);
  testOrd("001120313041", "e_(w^w^w^(e_0*2))", 2, 200, detail);
  testOrd("0011203131", "e_(e_1)", 2, 200, detail);
  testOrd("001120313131", "e_(e_2)", 2, 200, detail);
  testOrd("0011203140", "e_(e_w)", 2, 200, detail);
  testOrd("001120314050", "e_(e_(w^w))", 2, 200, detail);
  testOrd("001120314051", "e_(e_(e_0))", 2, 200, detail);
  testOrd("00112031405110", "(e_(e_(e_0)))w", 2, 200, detail);
  testOrd("00112031405120", "e_(e_(e_0)+1)", 2, 200, detail);
  testOrd("00112031405130", "e_((e_(e_0))w)", 2, 200, detail);
  testOrd("00112031405140", "e_(e_((e_0)w))", 2, 200, detail);
  testOrd("00112031405150", "e_(e_(w^((e_0)((e_0)w))))", 2, 200, detail);
  testOrd("00112031405160", "e_(e_(e_w))", 2, 200, detail);
  testOrd("0011203140516060", "e_(e_(e_(w^2)))", 2, 200, detail);
  testOrd("0011203140516070", "e_(e_(e_(w^w)))", 2, 200, detail);
  testOrd("0011203140516071", "e_(e_(e_(e_0)))", 2, 200, detail);
  testOrd("001121", "p0(p1(p1(0)))", 2, 200, detail);
  testOrd("00112111", "p0(p1(p1(0))+p1(0))", 2, 200, detail);
  testOrd("00112121", "p0(p1(p1(0)+p1(0)))", 2, 200, detail);
  testOrd("00112131", "p0(p1(p1(p1(0))))", 2, 200, detail);
  testOrd("001122", "p0(p1(p2(0)))", 2, 200, detail);
  testOrd("00112200", "p0(p1(p2(0)))+1", 2, 200, detail);
  testOrd("00112210", "(p0(p1(p2(0))))w", 2, 200, detail);
  testOrd("001122102132", "w^w^(p0(p1(p2(0)))*2)", 2, 200, detail);
  testOrd("00112211", "p0(p1(p2(0))+p1(0))", 2, 200, detail);
  testOrd("00112220", "p0(p1(p2(0)+p0(0)))", 2, 200, detail);
  testOrd("00112221", "p0(p1(p2(0)+p1(0)))", 2, 200, detail);
  testOrd("00112222", "p0(p1(p2(0)+p2(0)))", 2, 200, detail);
  testOrd("00112230", "p0(p1(p2(p0(0))))", 2, 200, detail);
  testOrd("00112231", "p0(p1(p2(p1(0))))", 2, 200, detail);
  testOrd("00112232", "p0(p1(p2(p2(0))))", 2, 200, detail);
  testOrd("00112233", "p0(p1(p2(p3(0))))", 2, 200, detail);
  testOrd("000111100000000", "(p0(pw(0)))w+2", 3, 200, detail);
  testOrd("000111100211200311", "w^w^w^(p0(pw(0))*2)", 3, 200, detail);
  testOrd("000111222", "(0,0,0)(1,1,1)(2,2,2)", 3, 200, detail);
  testOrd("0000111110002111", "w^w^((0,0,0,0)(1,1,1,1)*2)", 4, 200, detail);

  printf("Test completed without error.\n");
  return;
}

/****************************

 main - main function which is invoked at the beginning

 Commandline arguments are stored in argc and argv, which is
 read through the standard getopt function.

****************************/

int main(int argc, char *argv[]) {
  /**************************** Initialization ****************************/
  int *S, *Delta, *C, row = 0, bad, i = 0, m, nr, len;
  int opt = 1, ver, detail = 0, help = 0, test = 0;
  long n, nc, nn, num = 0, s = 20, step = 0, maxstep = 0;
  char *bm, arg;

  /* Read commandline options */
  ver = intVersion((char *)versionBM);
  while ((arg = getopt(argc, argv, "v:o:s:t:dTh")) != -1) {
    switch (arg) {
      case 'v':
        ver = intVersion(optarg);
        if (ver == 0) {
          printf("Error: invalid version.\n");
          help = 1;
        }
        break;
      case 'o':
        opt = atoi(optarg);
        break;
      case 's':
        s = atoi(optarg);
        break;
      case 't':
        maxstep = atoi(optarg);
        break;
      case 'd':
        detail = 1;
        break;
      case 'T':
        test = 1;
        break;
      case 'h':
        help = 1;
      default: /* '?' */
        help = 1;
    }
  }
  if (test) {
    testAll(detail);
    return 0;
  }

  if (opt < 1 || opt > 4) {
    printf("Error: invalid opt value.\n");
    help = 1;
  }
  if (s < 0) {
    printf("Error: Negative value of seq not allowed.\n");
    help = 1;
  }
  if (maxstep < 0) {
    printf("Error: Negative value of stp not allowed.\n");
    help = 1;
  }

  if (optind + 1 == argc) {
    bm = argv[optind];
  } else if (optind == argc) {
    if (help == 0) printf("Error: sequence not specified.\n");
    help = 1;
    bm = "";
  } else {
    if (help == 0) printf("Too many options.\n");
    help = 1;
    bm = "";
  }

  /* Read numbers of rows */
  len = strlen(bm);
  for (m = 0; m < len; m++) {
    if (bm[m] == ',') i++;
    if (bm[m] == ')') {
      if (row < i) row = i;
      i = 0;
    }
    if (bm[m] == '[') m = len;
  }
  /* nr = Number of rows */
  nr = row + 1;

  /* Allocate array */
  S = malloc(sizeof(int) * nr * s);
  Delta = malloc(sizeof(int) * nr);
  C = malloc(sizeof(int) * nr * s);
  if (S == NULL || Delta == NULL || C == NULL) {
    printf("Allocation failed.\n");
    exit(-1);
  }

  /* Read Bashicu Matrix */
  getMatrix(bm, S, nr, &nc, &num);
  n = nc - 1;

  if (num < 1) num = 2;
  if (n < 0 && help == 0) {
    printf("Error: invalid sequence of Bashicu matrix.\n");
    help = 1;
  }

  /*** Show help and exit ***/
  if (help) {
    showHelp();
    return 0;
  }

  /*** Initialize C matrix ***/
  for (m = 0; m <= row; m++) {
    C[m + nr] = 1;
  }

  /*** Check standard ***/
  i = chkStd(S, nc, nr, ver, detail);
  if (i && !detail) {
    printf("Not standard. Use detail option to see why.\n");
  }
  if (i == 0 && detail) {
    printf("--------------------------------------------\n");
    printf("Legend for ordinal: w = omega, e = epsilon, p = Buchholz psi\n");
  }

  /****************************
     Main loop of calculation
   ****************************/
  for (; n >= 0; n--) {
    if (detail) {
      printf("--------------------------------------------\n");
    }
    showSeq(S, nr, n + 1, num, 1); /* Show current sequence */

    /******* Calculation in 3 steps *******/
    /* Step 1: Increment number with f(n) */
    if (opt == 2) num++;
    if (opt == 3) num *= num;
    if (opt == 4 && S[n * nr] == 0) num++;

    /* Step 2: Determine the bad sequence and calculate Delta and C */

    bad = getBadSequence(S, Delta, C, ver, detail, n, nr);

    /* Step 3: Copy bad sequence */
    /* If opt=4, copy n-1; otherwise, copy n. nn = next n */
    nn = (opt == 4) ? n + bad * (num - 1) : n + bad * num;
    /* Check termination */
    if (nn > s) {
      printf("Length of sequence exceeds %ld at next step.\n", s);
      free(S);
      free(Delta);
      free(C);
      return 0;
    }
    if (maxstep && step++ >= maxstep) {
      printf("Maximum step of calculation %ld has reached.\n", maxstep);
      free(S);
      free(Delta);
      free(C);
      return 0;
    }

    /* Show detailed process (commandline option -d) */
    if (detail && bad) {
      showDetail(S, Delta, C, ver, n, nr, num, bad);
    }

    /* Copy bad sequence */
    copyBadSequence(S, Delta, C, ver, &n, nn, nr, bad);
  }
  /* Finished calculation */
  printf("Finished. Calculated number = %ld\n", num);
  free(S);
  free(Delta);
  free(C);
  return 0;
}
