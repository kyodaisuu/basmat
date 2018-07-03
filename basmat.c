/*

   basmat - Bashicu Matrix Calculator
*/
const char *version = "3.1-beta";
/*
   https://github.com/kyodaisuu/basmat

   See REAME.md for more information

***********************************/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *versionBM = "2"; /* Default version of BM */

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
      "       Available versions: 1, 2, 2.1, 2.2, 3\n"
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
      "See also https://github.com/kyodaisuu/basmat/blob/master/README.md\n");
  return;
}

int intVersion(char *version) {
  if (strcmp(version, "1.0") == 0 || strcmp(version, "1") == 0)
    return 100;
  else if (strcmp(version, "2.0") == 0 || strcmp(version, "2") == 0)
    return 200;
  else if (strcmp(version, "2.1") == 0)
    return 210;
  else if (strcmp(version, "2.2") == 0)
    return 220;
  else if (strcmp(version, "3.0") == 0 || strcmp(version, "3") == 0)
    return 300;
  else {
    return 0;
  }
}

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
}

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
/* argmin returns the index whose element is minimum of P in nr rows */
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
/* getConcestor returns the column number of the concestor of with m rows
   from the child column c in Bashicu matrix S consist of the nr rows
   (it returns -1 when concestor is not found) */
int getConcestor(int *S, int m, int c, int nr) {
  int r, maxr;
  int *P = malloc(sizeof(int) * nr);

  for (r = 0; r < m; r++) P[r] = getParent(S, r, c, nr);
  while (find(P, m, -1) == -1) {
    if (areAllEq(P, m)) {
      /* same */
      r = P[0];
      free(P);
      return r;
    } else {
      /* not same */
      maxr = argmax(P, m);
      P[maxr] = getParent(S, maxr, P[maxr], nr);
    }
  }
  free(P);
  return -1;
}

/* getBadSequence returns the length of the bad sequence as return variable.
Delta and C are directly returned using pointers */

int getBadSequence(int *S, int *Delta, int *C, int ver, int detail, long n,
                   int nr) {
  int bad = 0; /* Length of bad sequence */
  int found, j, l, m;
  int row = nr - 1;
  long k;

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
      for (k = 0; k <= n; k++) { /* k = pivot column */
        if (detail) {
          printf("S[n]-D= (");
          for (l = 0; l < row; l++) printf("%d,", S[l + n * nr] - Delta[l]);
          printf("%d)\n", S[row + n * nr] - Delta[row]);
        }
        if (detail) {
          printf("S[n-k]= (");
          for (l = 0; l < row; l++) printf("%d,", S[l + (n - k) * nr]);
          printf("%d)\n", S[row + (n - k) * nr]);
        }
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
        if (detail) {
          printf("Delta = (");
          for (l = 0; l < row; l++) printf("%d,", Delta[l]);
          printf("%d)\n", Delta[row]);
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
      if (detail) {
        printf("C     = ");
        for (l = 1; l <= bad; l++) {
          printf("(");
          for (m = 0; m < row; m++) printf("%d,", C[m + l * nr]);
          printf("%d)", C[row + l * nr]);
        }
        printf("\n");
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
#if 0
          printf("find tree parent:\n");
          printf("child =(");
          for (j=0; j<row; j++) printf ("%d,",S[j+(n-k)*nr]);
          printf ("%d)\n", S[j+(n-k)*nr]);
          printf("parent=(");
          for (j=0; j<row; j++) printf ("%d,",S[j+(n-k-l)*nr]);
          printf ("%d)\n", S[j+(n-k-l)*nr]);
#endif
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
#if 0
      printf("find label parent:\n");
      printf("child =(");
      for (j=0; j<row; j++) printf ("%d,",S[j+n*nr]);
      printf ("%d)\n", S[j+n*nr]);
      printf("parent=(");
      for (j=0; j<row; j++) printf ("%d,",S[j+(n-k)*nr]);
      printf ("%d)\n", S[j+(n-k)*nr]);
      printf("found=%d\n",found);
#endif
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
    }
  }
  return bad;
}

/* showDetail shows detailed process */
void showDetail(int *S, int *Delta, int *C, int ver, long n, int nr, long num,
                int bad) {
  int l, m;
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
  if (ver == 200) {
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

/* copyBadSequence(S, Delta, C, ver, &n, nn, nr, bad)
   Copy bad sequence */

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
  } else if (ver == 200) {
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

/* oneStep
   One step calculation ****************/

void oneStep(int *S, long *nc, int nr, long num, int ver, int opt) {
  int *Delta, *C, bad, m;
  int row = nr - 1;
  long nn;
  long n = *nc - 1;
  if (opt == 2) num++;
  if (opt == 3) num *= num;
  if (opt == 4 && S[n * nr] == 0) num++;
  int s = num * (*nc + 1);
  Delta = malloc(sizeof(int) * nr);
  C = malloc(sizeof(int) * nr * s);
  for (m = 0; m <= row; m++) {
    C[m + nr] = 1;
  }
  bad = getBadSequence(S, Delta, C, ver, 0, n, nr);
  nn = (opt == 4) ? n + bad * (num - 1) : n + bad * num;
  copyBadSequence(S, Delta, C, ver, &n, nn, nr, bad);
  *nc = n;
  free(Delta);
  free(C);
  return;
}

/* testOne
   Test One step */

void testOne(char *bm, char *result, int nr, int ver, long num, int opt) {
  int S[100], i, j, len;
  len = strlen(bm);
  long nc = len / nr;
  for (i = 0; i < len; i++) {
    S[i] = bm[i] - '0';
  }
  oneStep(S, &nc, nr, num, ver, opt);
  int err = 0;
  if (nc * nr == strlen(result)) {
    for (i = 0; i < nc * nr; i++) {
      if (result[i] != S[i] + '0') {
        err = 1;
      }
    }
    if (err == 0) {
      return;
    }
  }
  printf("Test failed with ");
  for (i = 0; i < len / nr; i++) {
    printf("(");
    for (j = 0; j < nr - 1; j++) printf("%d,", bm[j + i * nr] - '0');
    printf("%d)", bm[nr - 1 + i * nr] - '0');
  }
  printf("[%ld]\n", num);
  printf("ver = %d, opt = %d\n", ver, opt);
  printf("Input    = %s\nExpected = %s\nActual   = ", bm, result);
  for (i = 0; i < nc * nr; i++) {
    printf("%d", S[i]);
  }
  printf("\n");
  exit(-1);
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

/*
   Check Standard format

   S: Sequence
   nc: numbers of columns
   nr: numbers of rows
   ver: version
   detail: show detail

   return 0 if Standard
          1 if not
****************/

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
  int *SA, *Delta, *C, bad, num, nn, larger, smaller;
  long p2;
  SA = malloc(sizeof(int) * (row + 1) * nc * 2);
  Delta = malloc(sizeof(int) * (row + 1));
  C = malloc(sizeof(int) * (row + 1) * nc * 2);
  for (i = 0; i < row + 1; i++) {
    C[i + row + 1] = 1;
  }

  for (i = 0; i <= p; i++) {
    for (j = 0; j < row + 1; j++) {
      SA[j + i * (row + 1)] = i;
    }
  }
  if (detail) {
    showSeq(SA, row + 1, p + 1, 0, 1);
  }

  /* Check decreasing sequence of SA */
  while (1) {
    bad = getBadSequence(SA, Delta, C, ver, 0, p, row + 1);
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
        free(SA);
        free(Delta);
        free(C);
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
      free(SA);
      free(Delta);
      free(C);
      return 0;
    }
    p = p2;
    if (detail) {
      showSeq(SA, row + 1, p + 1, 0, 1);
    }
  }
}

/*
  Test standard format

*/

void testStd(char *bm, int expected, int nr, int ver) {
  int S[100], i, j, len, actual;
  len = strlen(bm);
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

/*********************

  Run this test by

    basmat -T

  Note that memory is allocated by S[100] in this test for simplicity.
  If you write a test, make it short or declare larger array.

**********************/

void test() {
  /* testOne(input, expected, nr, ver, num, opt) */

  /* Some basic calculations */
  testOne("0122", "0121212", 1, 100, 2, 1);
  testOne("0122", "012121212", 1, 200, 2, 2);
  testOne("0122", "01212121212", 1, 300, 2, 3);
  testOne("0122", "01212", 1, 210, 2, 4);
  testOne("0123", "012222222222", 1, 100, 3, 3);
  testOne("0011", "00102030", 2, 220, 3, 1);
  testOne("00001100", "0000100020003000", 4, 220, 3, 1);
  testOne("001100", "0011", 2, 200, 2, 4);
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
  testOne("000111200111", "000111200110221300", 3, 300, 1, 1);

  /* (0,0,0)(1,0,0)(2,0,0)(2,1,0)(3,1,1)[1] */
  testOne("000100200210311", "000100200210310320", 3, 100, 1, 1);
  testOne("000100200210311", "000100200210310400420", 3, 200, 1, 1);
  testOne("000100200210311", "000100200210310410420", 3, 210, 1, 1);
  testOne("000100200210311", "000100200210", 3, 220, 1, 1);
  testOne("000100200210311", "000100200210310400420", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,1,0)(1,1,1)[1] */
  testOne("000111210111", "000111210110221320", 3, 200, 1, 1);
  testOne("000111210111", "000111210110221320", 3, 210, 1, 1);
  testOne("000111210111", "000111210", 3, 220, 1, 1);
  testOne("000111210111", "000111210110221310", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,1,1)(1,1,1)[1] */
  testOne("000111211111", "000111211110221321", 3, 200, 1, 1);
  testOne("000111211111", "000111211110221321", 3, 210, 1, 1);
  testOne("000111211111", "000111211110221321", 3, 220, 1, 1);
  testOne("000111211111", "000111211110221311", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,2,2)(3,1,0)(2,2,1)[1] */
  testOne("000111222310221", "000111222310220331442530", 3, 200, 1, 1);
  testOne("000111222310221", "000111222310220331442530", 3, 210, 1, 1);
  testOne("000111222310221", "000111222310", 3, 220, 1, 1);
  testOne("000111222310221", "000111222310220331442510", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,2,2)(3,1,0)(2,2,2)[1] */
  testOne("000111222310222", "000111222310221332420", 3, 100, 1, 1);
  testOne("000111222310222", "000111222310221332410", 3, 200, 1, 1);
  testOne("000111222310222", "000111222310221332420", 3, 210, 1, 1);
  testOne("000111222310222", "000111222310", 3, 220, 1, 1);
  testOne("000111222310222", "000111222310221332410", 3, 300, 1, 1);

  /* (0,0,0)(1,1,1)(2,2,2)(3,1,1)(2,2,2)[1] */
  testOne("000111222311222", "000111222311221332421", 3, 100, 1, 1);
  testOne("000111222311222", "000111222311221332411", 3, 200, 1, 1);
  testOne("000111222311222", "000111222311221332421", 3, 210, 1, 1);
  testOne("000111222311222", "000111222311220331442531", 3, 220, 1, 1);
  testOne("000111222311222", "000111222311221332411", 3, 300, 1, 1);

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

  printf("Test completed without error.\n");
  return;
}

/**************************** Main ****************************/

int main(int argc, char *argv[]) {
  /**************************** Initialization ****************************/
  int *S, *Delta, *C, row = 0, bad, i = 0, m, nr, len;
  int opt = 1, ver, detail = 0, help = 0;
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
        test();
        return 0;
      case 'h':
        help = 1;
      default: /* '?' */
        help = 1;
    }
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

  /**************************** Start calculation ****************************/
  for (; n >= 0; n--) {
    if (detail) printf("--------------------------------------------\n");
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
