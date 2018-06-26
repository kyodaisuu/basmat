REM Bashicu Matrix latest version
REM Made by Bashicu
REM 21:32, June 12, 2018's version of Bashicu matrix number in this page
REM http://ja.googology.wikia.com/wiki/%E3%83%A6%E3%83%BC%E3%82%B6%E3%83%BC%E3%83%96%E3%83%AD%E3%82%B0:BashicuHyudora/BASIC%E8%A8%80%E8%AA%9E%E3%81%AB%E3%82%88%E3%82%8B%E5%B7%A8%E5%A4%A7%E6%95%B0%E3%81%AE%E3%81%BE%E3%81%A8%E3%82%81

A=9:dim B[∞,∞],C[∞]
for D=0 to 9
 for E=0 to A
  B[1,E]=1
 next
 for F=1 to 0 step -1
  A=A*A
  for G=0 to F
   for H=0 to E
    if B[F-G,H]<B[F,H]-C[H] | B[F,0]=0 then
     if B[F,H+1]=0 then H=E:I=G:G=F else C[H]=B[F,H]-B[F-G,H]
    else
     H=E
    endif
   next
  next
  for J=1 to A
   for K=1 to I
    B[F,0]=B[F-I,0]+C[0]
    for L=1 to E
     B[F,L]=B[F-I,L]
     for M=0 to F
      if B[F-M,0]<B[F,0] | K=1 then
       if B[F-I-M,L]<B[F-M,L] & B[F-M,L]<B[F,L]+C[L] | K=1 then B[F,L]=B[F,L]+C[L]
       M=F
      endif
     next
    next
    F=F+1
   next
  next
  for N=0 to E
   C[N]=0
  next
 next
next
print A
