# Definition of Bashicu Matrix

Bashicu Matrix has various versions. Original definition is summarized in [this blog post](http://googology.wikia.com/wiki/User:Kyodaisuu/BasmatVersion). BM1, 2, and 3 were written by Bashicu, and it was translated to C code by Fish. Version 2.1 and 2.2 were written with C code by koteitan. All the versions are in the source code of this C program.

Therefore, to understand the definition of Bashicu Matrix precisely one should read the source code of [basmat.c](https://github.com/kyodaisuu/basmat/blob/master/basmat.c), which has become very large because of implementing many features. Actually the core part of the definition is not very large, so one can read the code following the reader's guide in this page.

## Basic input and output

Before reading the code, one should study basic behaviour of this program. As written in the [toppage](index.md), take this example.

```
basmat -v 2 -o 4 -s 100 -t 5 "(0,0,0)(1,1,1)(2,0,0)(1,1,1)[2]"
```

It calculates the Bashicu matrix version 2 of (0,0,0)(1,1,1)(2,0,0)(1,1,1) with a parameter of 2 in the option of 4 (Hardy mode), with maximum length of sequence of 100 and the maximum step of calculation of 5. Then this is shown.

<pre>
(0,0,0)(1,1,1)(2,0,0)(1,1,1)[2]
(0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,2,1)(3,0,0)[2]
(0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,2,1)(2,2,1)[2]
(0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,2,1)(2,2,0)(3,3,1)[2]
(0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,2,1)(2,2,0)(3,3,0)[2]
(0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,2,1)(2,2,0)(3,2,0)[2]
Maximum step of calculation 5 has reached.
</pre>

In each step, Bashicu Matrix is calculated with a certain rule which is determined with the BM (0,0,0)(1,1,1)(2,0,0)(1,1,1) and the integer paramter [2]. The matrix grows very fast and the numbers increases (with options of 2, 3, 4), and it appears that the calculation never ends. However the claim of Bashicu matrix is that the length of the sequence will eventually decrease and finally a very large finite number is written. Whether the calculation always terminates is an open question.

## Initialization

So now let us start reading the [source code](https://github.com/kyodaisuu/basmat/blob/master/basmat.c). We start by reading the main function written at the last part, because it is the first part when the program starts.

```
int main(int argc, char *argv[]) {
  /**************************** Initialization ****************************/
  int *S, *Delta, *C, row = 0, bad, i = 0, m, nr, len;
  int opt = 1, ver, detail = 0, help = 0, test = 0;
  long n, nc, nn, num = 0, s = 20, step = 0, maxstep = 0;
  char *bm, arg;
```

It starts by declaring some variables. There are some types of variables, where int and long are integers (long has larger memory allocation) and char is character. Asterisc before the variable name, such as *S, means that the variable is an array. *bm is also an array because char is a one character and bm is a string which consists of many characters.

Then the program reads the commandline arguments in the block of

```
while ((arg = getopt(argc, argv, "v:o:s:t:dTh")) != -1) {
```

In this block getopt, a standard function of C is used, and commandline arguments are read to following parameters.

* bm - Bashicu Matrix string such as (0,0,0)(1,1,1)(2,0,0)(1,1,1)[2]
* ver - version of BM
* opt - option for incrementing n
* s - maximum length of sequence
* t - maximum step of calculation
* d - to show detail or not (1 = show, 0 = no show)

when -T or -h option is in the option, it just performs test or show help and exit.

After that, it reads the numbers of row of BM (Bashicu Matrix) from the string of bm, allocate memory of S, Delta and C (we need s and the numbers of row to determine the memory size), and read Bashicu Matrix by

```
/* Read Bashicu Matrix */
getMatrix(bm, S, nr, &nc, &num);
```

This is a function defined previously in basmat.c. bm is the input parameter and we get S, nc and num. Here "&" before nc and num means that it is a pointer to the variable. In this way we can get the value of nc and num as a return variable. Getting into the detail of pointer is not a focus of this article so please refer to appropriate site of C language. Anyway we get following parameters from the input string of bm.

* S - BM as array of integers
* nr - numbers of rows
* nc - numbers of columns
* num - integer parameter of BM.

For example, for bm string of (0,0,0)(1,1,1)(2,0,0)(1,1,1)[2], we get

* S = {0,0,0,1,1,1,2,0,0,1,1,1}
* nr = 3
* nc = 4
* num = 2

So how can we get a specific value of the sequence? If we want to get 3rd row 2nd column, we calculate 3+(2-1)*3 = 6th number of the S. Actually in C language, index is counted from 0. Therefore it is obtained by S[5]. By using the convention, we are going to index the column and row starting from 0. Therefore normal "3rd row and 2nd column" is "row 2 and column 1". In this way, a specific value of the BM can be obtained as follows.

* row i and column j = S[i+nr*j] (i, j indexed from 0)

As we use the index starting from 0, it is convenient to also prepare these parameters and they are sometimes used.

* row = nr-1 (largest row index)
* n = nc-1 (largest column index)

```
/*** Initialize C matrix ***/
for (m = 0; m <= row; m++) {
  C[m + nr] = 1;
}
```

This block initializes the C matrix to be used in the calculation of BM2.

```
/*** Check standard ***/
i = chkStd(S, nc, nr, ver, detail);
if (i && !detail) {
  printf("Not standard. Use detail option to see why.\n");
}
```

This block checks if the BM is standard form. Standard form is a BM which is expected to arise from calculation of (0,0,...,0)(1,1,...,1). When it is not standard, there is no chance that the BM appears in the calculation of (0,0,...,0)(1,1,...,1). It is an important property of a BM and therefore if it is not standard it is shown at the beginning to get attention.

Now the initialization finished.

## Main calculation loop

Now the calculation starts. Main calculation loop starts from

```
/**************************** Start calculation ****************************/
for (; n >= 0; n--) {
```

This is a loop of the variable n, which is "numbers of columns - 1". n is already initialized, and therefore initialization of n is omitted. n decreases every time of the loop (n--) and the loop continues while n>=0. Therefore it appears that it is just a simple n+1 time loop, but actually it isn't. Remember that n represents the numbers of columns, which normally "increases" every times of calculation. Actually n increases in the loop. However, just like a [Hydra game](http://googology.wikia.com/wiki/Kirby-Paris_hydra), it is expected that the calculation terminates at last.

When the calculation terminates,

```
/* Finished calculation */
printf("Finished. Calculated number = %ld\n", num);
free(S);
free(Delta);
free(C);
return 0;
```

it just print the calculated number "num" and finish the program.

Now let us look at the calculation procedure inside the loop.

```
showSeq(S, nr, n + 1, num, 1); /* Show current sequence */
```

This calls the function showSeq which shows the current sequence such as (0,0,0)(1,1,1)(2,0,0)(1,1,1)[2].

```
 /******* Calculation in 3 steps *******/
 /* Step 1: Increment number with f(n) */
 if (opt == 2) num++;
 if (opt == 3) num *= num;
 if (opt == 4 && S[n * nr] == 0) num++;
 ```

It takes care of the increment of n. opt=1 is constant n, opt=2 is n=n+1, opt=3 is n=n*n, opt=4 is Hardy mode where the n is incremented only when it is successor ordinal, which means that the rightmost sequence is 0.

```
 /* Step 2: Determine the bad sequence and calculate Delta and C */

 bad = getBadSequence(S, Delta, C, ver, detail, n, nr);
```

This is a core part of the Bashicu Matrix algorithm. The function getBadSequence determines the "length of bad part" as a return variable and it is stored in bad. Delta and C are also determined. The algorithm is explained in later part but here we just use the calculation result of

```
basmat -v 2 -d -o 4 -s 100 -t 5 "(0,0,0)(1,1,1)(2,0,0)(1,1,1)[2]"
```

which is

<pre>
Delta = (1,1,0)
C     = (1,1,1)(1,1,1)(1,0,0)
G = empty
B = (0,0,0)(1,1,1)(2,0,0)
</pre>

The calculation starts by "cutting" the rightmost column (1,1,1). Then the BM of (0,0,0)(1,1,1)(2,0,0) remains. Then it determines the "bad part" from the right of the sequence. Then the remaining part is the "good part". So the BM (0,0,0)(1,1,1)(2,0,0) is concatination of "good part" and "bad part". In this case bad part is determined as B = (0,0,0)(1,1,1)(2,0,0), which means that the "length of the bad part" is 3, and bad=3 is stored. Delta and C are also determined which is used in the next step. C is only used in BM2, and remaining part is almost same in other versions, except for the algorithm of how to determine the bad part and Delta.

```
 /* Step 3: Copy bad sequence */
 /* If opt=4, copy n-1; otherwise, copy n. nn = next n */
 nn = (opt == 4) ? n + bad * (num - 1) : n + bad * num;
```

Now we copy the bad part. When opt=1,2,3, the bad part is copied num times. when opt=4, the bad part is copied num-1 times. So in the above example, num=2 and opt=4, and therefore bad part is copied 1 time. So the original sequence is the concatination of

G + B + r

where r is the rightmost column which was cut first (1,1,1), and after the bad part is copied it will be like

G + B + B'

beause B is copied. Here B becomes B' because it changes with Delta and C. Here nn is a parameter which stores the numbers of columns of BM after the copy. It was n+1 before it was copied. It changes to n when the rightmost column is cut, and it increases by bad * (num - 1) or bad * num depending on the opt parameters.

Then termination is checked. If nn exceeds s, the maximum length of calculation, or the step of calculation, step, exceeds maxstep, maximum step of calculation, the program terminates.

After that, detailed process is shown by

```
 /* Show detailed process (commandline option -d) */
 if (detail && bad) {
   showDetail(S, Delta, C, ver, n, nr, num, bad);
 }
```

it is shown when detail option is designated and there was a bad part (bad is not 0).

```
 /* Copy bad sequence */
 copyBadSequence(S, Delta, C, ver, &n, nn, nr, bad);
```

Finally it copies the bad part by copyBadSequence function.

## Finding a bad part

Now we are going to read getBadSequence function.

```
int getBadSequence(int *S, int *Delta, int *C, int ver, int detail, long n, int nr) {
```

As we have reviewed so far,

* S is the current sequene
* ver is the version of BM
* n is the index of the rightmost column
* nr is the numbers of rows

Delta and C will be calculated in this function. Memory is already allocated and C is already initialized.

After some variable declaration, including

* row = nr-1 is the index of the last row

it starts with

```
if (S[n * nr] == 0) {
  /* If the first row of S_n is 0, no bad sequence */
  return 0;
} else {
```

As it is written in the comment, if the first row of the rightmost column is 0, there is no "bad part" and threrefore the length of the bad part is 0.

After that, the calculation is different for different version of BM. In this article, we focus on BM version 2. Therefore we start reading from

```
} else if (ver == 200) {
  /***** Version 2 *****/
```

At first we initialize Delta with 0 by

```
  /* Clear Delta */
  for (m = 0; m <= row; m++) Delta[m] = 0;
```

Then we determine the bad part and calculate Delta in this block.

```
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
```

k is the "pivot column". Although the loop of `for (k = 0; k <= n; k++)` means that k increases from 0 to n, actually k is always used as "n-k" in the program. Therefore actually "n-k"th column is the pivot column, and the pivot column n-k goes from rightmost to leftmost.

Inside the loop of the pivot column, we have another loop of `for (l = 0; l <= row; l++)`. In this loop l is the index of the row. So we examine all the numbers in the pivot column.

In this loop, we examine

```
if (S[l + (n - k) * nr] < S[l + n * nr] - Delta[l])
```

S[l + (n - k) * nr] is the l-th row, (n-k)-th column (pivot column). Remember that index starts from 0. S[l + n * nr] is the l-th row, n-th column. Delta[l] is the Delta of l-th row. If it is not false, it goes to `l = row; /* Go to left sequence (k loop) */`, meaning that we exits the l-loop by setting l=row, and go to the k-loop, meaning that the pivot column goes left.

If the above if statement is true, we make another comparison of

```
if (S[l + 1 + n * nr] == 0 || l == row)
```

It checks if the (l+1)-th row, n-th column is 0. If we are at the bottom of the row (l==row), then it is also true. In this case, "bad=k" is performed and we get out of l loop and k loop, meaning that we found the "bad part" and the numbers of the bad part is bad. As we are comparing (n-k)-th column, n is the rightmost column where it will be "cut off" anyway, when the "leftmost column index" of the bad part is n-k, the numbers of the bad part is k.

If the above if statement is false, Delta[l] is calculated as

```
Delta[l] = S[l + n * nr] - S[l + (n - k) * nr];
```

and l loop and k loop continues.

Now we finished the k loop and we are going to calculate the C matrix.

```
  /* Calculate C matrix */
  for (k = 1; k <= bad; k++) {
```

C matrix is actually defined as the "first column of the bad part" as index of 1, and first row as the index of 0. Therefore the loop starts from 1 and ends at the length of the bad part, k.

Then inside the k loop we have this block.

```
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
```

It is difficult to translate what this block is doing to a human language, so I just show the meaning of each sentence.

```
for (l = k; l >= 0; l--) {
```

This is a for block starting from k to 0 decreasing.

```
if (S[(n - bad + l) * nr] < S[(n - bad + k) * nr])
```

This is a simple if statement.

```
for (m = 0; m <= row; m++)
```

There is another for loop in the if statement.

The next statment is a complex statement. It first checks that both of the following statements are true

* S[m + (n - bad) * nr] < S[m + (n - bad + k) * nr]
* C[m + (l + 1) * nr] == 1

and if both of them are true,

C[m + (k + 1) * nr] = 1

if it is false,

C[m + (k + 1) * nr] = 0

so it means that value of the C[m + (k + 1) * nr] depends on the if statement above. Here please note that c matrix always have +1 in the column part, such as C[m + (l + 1) * nr]. As I wrote, C matrix starts index of 1 as the column, and therefore 1 should be added. Therefore C[m + (l + 1) * nr] is the m-th row, l-th column of the C matrix.

## Copy the bad part

Now we are going to read the function to copy the bad part.

```
void copyBadSequence(int *S, int *Delta, int *C, int ver, long *n, long nn, int nr, int bad) {
```

We just read the version 2.

```
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
```

It copies the bad part by incrementing with the value of Delta[l] and C matrix. For example, for

<pre>
(0,0,0)(1,1,1)(2,0,0)(1,1,1)[2]
Delta = (1,1,0)
C     = (1,1,1)(1,1,1)(1,0,0)
G = empty
B = (0,0,0)(1,1,1)(2,0,0)
</pre>

next to (0,0,0)(1,1,1)(2,0,0), (0,0,0) is copied, with the Delta increment of (1,1,0), therefore it becomes (0+1,0+1,0+0) = (1,1,0). The first column of C matrix is (1,1,1) so all the Delta was added. In the same way, second column (1,1,1) is copied with the Delta of (1,1,0) so it becomes (2,2,1). Then when the third column (2,0,0) is copied, C matrix is (1,0,0) and therefore Delta * C is (1,0,0), and (3,0,0) is copied. As a result, (0,0,0)(1,1,1)(2,0,0)(1,1,1)(2,2,1)(3,0,0) is obtained.

Now I hope you understand how to read the source code of Bashcu Matrix system.

----
[Home](index.md)
