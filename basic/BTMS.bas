REM Bashicu Triangular Matrix
REM Made by Bashicu
REM 17:38, October 20, 2018 (UTC+0900)'s version of Bashicu triangular matrix number in this page
REM http://ja.googology.wikia.com/wiki/%E3%83%A6%E3%83%BC%E3%82%B6%E3%83%BC%E3%83%96%E3%83%AD%E3%82%B0:BashicuHyudora/BASIC%E8%A8%80%E8%AA%9E%E3%81%AB%E3%82%88%E3%82%8B%E5%B7%A8%E5%A4%A7%E6%95%B0%E3%81%AE%E3%81%BE%E3%81%A8%E3%82%81

A=99:dim B[∞,∞],C[∞,∞],D[∞],E[∞],F[∞]
for G=0 to 99
 for H=1 to A
  for I=1 to H
   B[H,I]=H-I
  next
 next
 for I=A to 1 step -1
  A=pow(A,A)
  for J=0 to I-1
   for K=1 to H
    if B[I-J,K]<B[I,K]-D[K] | B[I,1]=0 then
     if B[I,K+1]=0 then
      if 0<L then
       for N=0 to E[1]
        for O=1 to H
         P=0
         if 0<C[E[L]-E[1]+N+1,O] then P=B[I-E[C[E[L]-E[1]+N+1,O]],O]-B[I-J,O]
         if B[I-J+N,O]<B[I-E[1]+N,O]-P then J=I
         if B[I-E[1]+N,O]-P<B[I-J+N,O] | J=I then O=H:N=J
        next
       next
      endif
      if J<I then
       L=L+1:E[L]=J:M=J+2
       for N=1 to L
        for O=1 to H
         if K<O then C[J-E[N]+1,O]=0 else C[J-E[N]+1,O]=N
        next
        for O=E[N] to 1 step -1
         for P=1 to H
          if J-O+2<M | C[J-O+2,P]=0 then
           for Q=O to E[N]
            for R=1 to P
             if B[I-Q,R]<B[I-O+1,R]-F[R] then
              if B[I-Q+1,R+1]=0 | R=P then S=Q:T=R:R=P:Q=J else F[R]=B[I-O+1,R]-B[I-Q,R]
             else
              R=H
             endif
            next
           next
           if 0<C[J-S+1,P] & T=P then C[J-O+2,P]=N else C[J-O+2,P]=0
           for Q=1 to H
            F[Q]=0
           next
           T=0
          endif
         next
        next
        M=J-E[N]+1
       next
      endif
      if K=1 | 1<B[I,K] then J=I
      K=H
     else
      D[K]=B[I,K]-B[I-J,K]
     endif
    else
     K=H
    endif
   next
  next
  for J=1 to H
   if 0<B[I,J+1] then D[J]=B[I,J]-B[I-E[L],J]
  next
  for J=1 to A
   for K=1 to E[L]
    for M=1 to H
     if 0<C[K,M] then B[I,M]=B[I-E[L],M]+D[M] else B[I,M]=B[I-E[L],M]
    next
    I=I+1
   next
  next
  for J=1 to H
   D[J]=0
  next
  L=0
 next
next
print A
