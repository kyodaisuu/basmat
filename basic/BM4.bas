REM Bashicu Matrix version 4
REM Made by Bashicu
REM 15:13, September 1, 2018's version of Bashicu matrix number in this page
REM http://ja.googology.wikia.com/wiki/%E3%83%A6%E3%83%BC%E3%82%B6%E3%83%BC%E3%83%96%E3%83%AD%E3%82%B0:BashicuHyudora/BASIC%E8%A8%80%E8%AA%9E%E3%81%AB%E3%82%88%E3%82%8B%E5%B7%A8%E5%A4%A7%E6%95%B0%E3%81%AE%E3%81%BE%E3%81%A8%E3%82%81

A=9:dim B[∞,∞],C[∞,∞],D[∞],E[∞]
for F=0 to 9
 for G=0 to A
  B[1,F]=1:C[1,F]=1
 next
 for H=1 to 0 step -1
  A=A*A
  for I=0 to H
   for J=0 to G
    if B[H-I,J]<B[H,J]-D[J] | B[H,0]=0 then
     if B[H,J+1]=0 then J=G:K=I:I=H else D[J]=B[H,J]-B[H-I,J]
    else
     J=G
    endif
   next
  next
  for L=K to 2 step -1
   for M=0 to G
    for N=L to K
     for O=0 to M
      if B[H-N,O]<B[H-L+1,O]-E[O] then
       if B[H-L+1,O+1]=0 | O=M then P=N:Q=O:O=M:N=K else E[O]=B[H-L+1,O]-B[H-N,O]
      else
       O=M
      endif
     next
    next
    if C[K-P+1,M]=1 & Q=M then C[K-L+2,M]=1 else C[K-L+2,M]=0
    for R=0 to G
     E[R]=0
    next
    Q=0
   next
  next
  for S=1 to A
   for T=1 to K
    for U=0 to G
     if C[T,U]=1 then B[H,U]=B[H-K,U]+D[U] else B[H,U]=B[H-K,U]
    next
    H=H+1
   next
  next
  for V=0 to G
   D[V]=0
  next
 next
next
print A
