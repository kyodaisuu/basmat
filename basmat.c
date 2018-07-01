/*

basmat - Bashicu Matrix Calculator version 3

https://github.com/kyodaisuu/basmat

See REAME.md

***********************************/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

void printM(int *A, int nr, int nc){
  int r,c;
  for(c=0;c<nc;c++){
    printf("(");
    for(r=0;r<nr-1;r++)printf("%d,",A[r+c*nr]);
    if(nr>0)printf("%d)",A[nr-1+c*nr]);
  }
}
void printA(int *A, int nr){
  int r=0;
  printf("(");
  for(r=0;r<nr-1;r++)printf("%d,",A[r]);
  if(nr>0)printf("%d)",A[nr-1]);
}
/* returns index of the element x in P with nr rows
   (it returns -1 when x is not found) */
int find(int *P, int nr, int x){
  int i;
  for(i=0;i<nr;i++) if(P[i]==x) return i;
  return -1;
}
/* are the all elements equal in nr rows of P */
int areAllEq(int *P, int nr){
  int i, p0=P[0];
  for(i=1;i<nr;i++) if(P[i]!=p0) return 0;
  return -1;
}
/* argmin returns the index whose element is minimum of P in nr rows */
int argmax(int *P, int nr){
  int i, m=nr;
  for(i=0;i<nr;i++) if(P[i]>P[m]) m=i;
  return m;
}
/* getParent returns the column number of the parent of the child column c
   in row r in Bashicu matrix S consist of the nr rows
   (it returns -1 when parent is not found) */
int getParent(int *S, int r, int c, int nr){
  int i;
  for(i=c;i>=0;i--) if(S[r+i*nr]<S[r+c*nr]) return i;
  return -1;
}
/* getConcestor returns the column number of the concestor of with m rows
   from the child column c in Bashicu matrix S consist of the nr rows
   (it returns -1 when concestor is not found) */
int getConcestor(int *S, int m, int c, int nr){
  int r, maxr;
  int *P = malloc(sizeof(int) * nr);

  for(r=0;r<m;r++) P[r] = getParent(S,r,c,nr);
  while(find(P,m,-1)==-1){
    if(areAllEq(P,m)){
      /* same */
      r = P[0];
      free(P);
      return r;
    }else{
      /* not same */
      maxr = argmax(P,m);
      P[maxr]=getParent(S,maxr,P[maxr],nr);
    }
  }
  free(P);
  return -1;
}

int main(int argc, char *argv[]) {
  /**************************** Initialization ****************************/
  int *S, *Delta, *C, row=0, bad, found, i=0, j, l, m, nr, len, K, L, M;
  int opt=1, ver=300, detail=0, help=0;
  long n, nn, k, num=0, s=20, step=0, maxstep=0;
  char *bm, arg;
  /* Read commandline options */

 while ((arg = getopt(argc, argv, "v:o:s:t:dh")) != -1) {
    switch (arg) {
        case 'v':
          if     (strcmp(optarg,"1.0")==0||strcmp(optarg,"1")==0)ver=100;
          else if(strcmp(optarg,"2.0")==0||strcmp(optarg,"2")==0)ver=200;
          else if(strcmp(optarg,"2.1")==0                       )ver=210;
          else if(strcmp(optarg,"2.2")==0                       )ver=220;
          else if(strcmp(optarg,"3.0")==0||strcmp(optarg,"3")==0)ver=300;
          else{
            printf("Error: invalid version.\n");
            help=1;
          }
        break;
        case 'o': opt=atoi(optarg); break;
        case 's': s=atoi(optarg); break;
        case 't': maxstep=atoi(optarg); break;
        case 'd': detail=1; break;
        case 'h': help=1;
        default: /* '?' */
            help=1;
    }
  }

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
  S     = malloc(sizeof(int) * nr * s);
  Delta = malloc(sizeof(int) * nr);
  C     = malloc(sizeof(int) * nr * s);
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
    printf ("basmat - Bashicu Matrix Calculator version 3.0\n"
    "Usage: basmat [-v ver] [-o opt] [-s seq] [-t stp] [-dh] ini\n\n"
    "  ini  Initial variable in the form of BM[n].\n"
    "       BM sequence expression of Bashicu matrix.\n"
    "       n: natural number. n=2 if not given.\n"
    "       ex. \"(0,0)(1,1)[3]\"   (Quote to escape brackets in shell)\n"
    "  ver  Version of Bashicu matrix system. Default = 3.\n"
    "       Available versions: 1, 2, 2.1, 2.2, 3\n"
    "  opt  Calculation option.\n"
    "       opt = 1: n is constant. (Default)\n"
    "       opt = 2: n = n+1 for each loop.\n"
    "       opt = 3: n = n*n for each loop.\n"
    "       opt = 4: Simulate Hardy. n=n+1 for successor, and copy n-1.\n"
    "  seq  Maximum length of sequence. Default = 20.\n"
    "  stp  Maximum step of calculation. Default = 0 (no limit).\n"
    "   -d  Show detailed process of calculation.\n"
    "   -h  Show this help and exit.\n\n"
    "Web interface is available at http://gyafun.jp/ln/basmat.cgi\n"
    "See also https://github.com/kyodaisuu/basmat/blob/master/README.md\n");
    return 0;
  }

  /*** Initialize C matrix ***/
  for (m = 0; m<=row; m++) {
    C[m + nr] = 1;
  }

  /**************************** Start calculation ****************************/
  for (n = j-1; n >= 0; n--) {
    if(detail)printf("--------------------------------------------\n");
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
      if (ver == 100) {
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
      } else if(ver == 200){
        /***** Version 2 *****/
        /* Clear Delta */
        for (m=0; m<=row; m++) Delta[m]=0;
        /* Determine the bad sequence and calculate Delta */
        for (k=0; k<=n; k++) { /* k = pivot column */
          if(detail){
            printf ("S[n]-D= (");
            for (l=0; l<row; l++) printf ("%d,",S[l+n*nr]-Delta[l]);
            printf ("%d)\n", S[row+n*nr]-Delta[row]);
          }
          if(detail){
            printf ("S[n-k]= (");
            for (l=0; l<row; l++) printf ("%d,",S[l+(n-k)*nr]);
            printf ("%d)\n", S[row+(n-k)*nr]);
          }
          for (l = 0; l <= row; l++) { /* l = row */
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
          if(detail){
            printf ("Delta = (");
            for (l=0; l<row; l++) printf ("%d,",Delta[l]);
            printf ("%d)\n", Delta[row]);
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
        if(detail){
            printf ("C     = ");
            for (l=1; l<=bad; l++) {
                printf ("(");
                for (m=0; m<row; m++) printf ("%d,",C[m+l*nr]);
                printf ("%d)",C[row+l*nr]);
            }
            printf ("\n");
        }
      }else if(ver == 210){
        /***** Version 2.1 *****/
        /* Calculate m = Uppermost zero at the rightmost sequence */
        m = row+1; /* row = number of rows + 1 */
        for (j=0; j<=row; j++){
          if (S[j + n*nr] == 0) m=j, j=row+1;
        }
        /* Determine the bad sequence and calculate Delta */
        for (k=0; k<=n; k++) {
          /* find and jump to parent of n */
          for (l=1; n-(k+l)>=0; l++) {
            if(S[0+(n-(k+l))*nr] < S[0+(n-k)*nr]){
#if 0
              printf("find tree parent:\n");
              printf("child =(");
              for (j=0; j<row; j++) printf ("%d,",S[j+(n-k)*nr]);
              printf ("%d)\n", S[j+(n-k)*nr]);
              printf("parent=(");
              for (j=0; j<row; j++) printf ("%d,",S[j+(n-k-l)*nr]);
              printf ("%d)\n", S[j+(n-k-l)*nr]);
#endif
              k=k+l; /* jump to parent */
              break;
            }
          }
          found = 1;
          for (l = 0; l < m; l++){
            if (S[l + (n-k)*nr] >= S[l + n*nr]){
              found=0;
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
            for (l = 0; l<=row; l++){
              Delta[l] = (l < m-1)? S[l + n*nr]-S[l + (n-k)*nr]: 0;
            }
            bad = k;
            k = n+1;
            if(detail){
              printf("S[bad=%ld]=(",(n-bad));
              for (l=0; l<row; l++) printf ("%d,",S[l+(n-bad)*nr]);
              printf ("%d)\n", S[row+(n-bad)*nr]);
            }
          }else{k=k-1;}
        }

      }else if(ver == 220){
        /***** Version 2.2 *****/
        /* Calculate m = Uppermost zero at the rightmost sequence */
        m = row+1; /* row = number of rows + 1 */
        for(j=0; j<=row; j++){
          if (S[j + n*nr] == 0) m=j, j=row+1;
        }
        /* Determine the bad sequence and calculate Delta */
        l=getConcestor(S,m,n,nr);
        if(l>=0){
          bad=n-l;
        }else{
          /* no concestor */
          bad=0;
        }
        /* If all dimension are small, calculate Delta */
        for(j=0; j<=row; j++){
          Delta[j] = (j<m-1) ? S[j+n*nr]-S[j+(n-bad)*nr]:0;
        }

      }else if(ver == 300){
        /***** Version 3 *****/
        /* Clear Delta */
        for (m=0; m<=row; m++) Delta[m]=0;
        /* Determine the bad sequence and calculate Delta */
        /* Note that this part is same as version 2 */

        for (k=0; k<=n; k++) { /* k = pivot column */
          for (l = 0; l <= row; l++) { /* l = row */
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
        if (ver == 200) {
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
    if (ver == 100 || ver == 210 || ver == 220) {
    /***** Version 1 *****/
      while (n < nn) {
        for (l=0; l<=row; l++) S[l + n*nr] = S[l + (n-bad)*nr] + Delta[l];
        n++;
      }
    } else if(ver == 200){
      /***** Version 2 *****/
      m = 1;
      while (n < nn) {
        for (l=0; l<=row; l++) {
          S[l + n*nr] = S[l + (n-bad)*nr] + Delta[l] * C[l + m*nr];
        }
        m++; n++;
        if (m > bad) m=1;
      }
    } else if(ver == 300){
      /***** Version 3 *****/
      K = 1; /* Equivalent to K in original BASIC code */
      while (n < nn) {
        for (L=0; L<=row; L++) {
          /* L is equivalent to L in the original BASIC code */
          S[L + n*nr] = S[L + (n-bad)*nr];
          /* Add Delta in certain conditions */
          for (m=0; m<=n; m++) {
            if (S[(n-m)*nr] < S[n*nr]) {
              M = m; /* Now M is determined which is equivalent to BASIC code when condition is met */
              break;
            }
          }
          if (L==0 || K==1 || (S[L+(n-bad-M)*nr] < S[L+(n-M)*nr] && S[L+(n-M)*nr] < S[L+n*nr]+Delta[L])) {
             /* Add Delta in this condition */
             S[L + n*nr] = S[L + n*nr] + Delta[L];
          }
          /* Finished adding Delta */
        }
        K++; n++;
        if (K > bad) K=1;
      }
    }
  }
  /* Finished calculation */
  printf ("Finished. Calculated number = %ld\n",num);
  free(S); free(Delta); free(C); return 0;
}
