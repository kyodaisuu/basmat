REM Bashicu Triangular Matrix
REM Made by Bashicu
REM 01:06, August 2, 2018's version of Bashicu triangular matrix number in this page
REM http://ja.googology.wikia.com/wiki/%E3%83%A6%E3%83%BC%E3%82%B6%E3%83%BC%E3%83%96%E3%83%AD%E3%82%B0:BashicuHyudora/BASIC%E8%A8%80%E8%AA%9E%E3%81%AB%E3%82%88%E3%82%8B%E5%B7%A8%E5%A4%A7%E6%95%B0%E3%81%AE%E3%81%BE%E3%81%A8%E3%82%81

A=9:dim B[∞,∞],C[∞,∞],D[∞]
for E=0 to 9
 for F=1 to A
  B[F,1]=F:C[F,1]=1
  for G=2 to F
   B[F,G]=1
  next
 next
 for H=A to 1 step -1
  A=pow(A,A)
  for I=0 to H-1
   for J=1 to G
    if B[H-I,1]<B[H,1]-K & J=1 | B[H-I,J]<B[H,J] & 1<J then
     if B[H,J+1]=0 & (J=1 | B[H-I,J+1]=0) then L=I:M=J:I=H:J=G else K=B[H,1]-B[H-I,1]
    else
     J=G
    endif
   next
  next
  for N=2 to L
   C[N,1]=1
   for O=2 to G
    for P=0 to H-L+N-1
     if B[H-L+N-P-1,1]<B[H-L+N-1,1] then
      if (C[N-P,O]=1 | C[N,2]=1) & B[H-L,O]<B[H-L+N-1,O] & O<M & C[N,O-1]=1 then
       C[N,O]=1
      else
       C[N,O]=0
      endif
      P=H
     endif
    next
   next
  next
  for Q=0 to H-1
   if B[H-Q,1]<B[H,1] then
    if M=2 | 1<B[H,M] then Q=0
    for R=1 to M-1
     D[R]=B[H-Q,R]-B[H-L,R]
    next
    if 2<M & 1=B[H,M] then S=1 else S=0
    Q=H
   endif
  next
  for T=1 to A
   for U=1+S to L
    for V=1 to G
     if C[U,V]=1 then B[H,V]=B[H-L+S,V]+D[V] else B[H,V]=B[H-L+S,V]
    next
    H=H+1
   next
  next
  K=0:L=0
  for W=1 to G
   D[W]=0
  next
 next
next
print A
