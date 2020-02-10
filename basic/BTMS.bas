REM Bashicu Triangular Matrix
REM Made by Bashicu
REM 10:34, February 10, 2020‎ (UTC+0900)'s version of Bashicu triangular matrix number in this page
REM https://googology.wikia.org/ja/wiki/%E3%83%A6%E3%83%BC%E3%82%B6%E3%83%BC%E3%83%96%E3%83%AD%E3%82%B0:BashicuHyudora/BASIC%E8%A8%80%E8%AA%9E%E3%81%AB%E3%82%88%E3%82%8B%E5%B7%A8%E5%A4%A7%E6%95%B0%E3%81%AE%E3%81%BE%E3%81%A8%E3%82%81

A=99:dim B[∞,∞],B1[∞,∞],C[∞],C1[∞]
for D=0 to 99
 for D1=1 to A
  for D2=1 to A
   B[D1,D2]=D1-D2
  next
 next
 for D3=A to 1 step -1
  A=pow(A,A)
  for D4=1 to D2
   if 0<B[D3,D4] & B[D3,D4+1]=0 then
    for D6=0 to D3-1
     for D7=1 to D4
      if B[D3-D6,D7]<B[D3,D7]-C[D7] then
       if D7<D4 then
        C[D7]=B[D3,D7]-B[D3-D6,D7]
       else
        if D8=0 then D8=D6
         D9=D9+1
         for D10=1 to D4
          B1[D3-D6,D10]=D9
         next
         for D11=1 to D4
          for D12=D3-D6+1 to D3
           for D13=D12 to D3-D6 step -1
            for D14=1 to D11
             if B[D13,D14]<B[D12,D14]-C1[D14] then
              if D11=D14 then
               if 0<B1[D13,D11] then B1[D12,D11]=D9
               D14=D11:D13=D3-D6
              else
               C1[D14]=B[D12,D14]-B[D13,D14]
              endif
            endif
           next
          next
          for D15=1 to D4
           C1[D15]=0
          next
         next
        next
        for D16=0 to D8
         for D17=1 to D2
          D18=0
          if 0<B1[D3-D8+D16,D17] then
           if D17<D4 then
            D18=B[D3-D8,D17]-B[D3-D6,D17]
           elseif D17=D4 & 0<B[D3-D8+16,D4]-C[D4]
            D18=C[D4]
           endif
          endif
          if B[D3-D6+D16,D17]<B[D3-D8+D16,D17]-D18 then
           if D17=D4 & C[D4]+1<B[D3,D4]-B[D3-D6,D4] & B[D3-D8,D4+1]=0 then C[D4]=C[D4]+1:D19=D6 else D6=D3-1
           D16=D8:D17=D2:D20=1
          elseif B[D3-D8+D16,D17]-D18<B[D3-D6+D16,D17]
           D17=D2
          endif
         next
        next
        if D20=0 then D19=D6 else D20=0
       endif
      else
       D7=D4
      endif
     next
    next
    D4=D2
   endif
  next
  for D21=1 to D2
   if 0<B[D3,D21+1] then C[D21]=B[D3,D21]-B[D3-D19,D21]
  next
  for D22=1 to A*D19
   for D23=1 to D2
    if 0<B1[D3-D19,D23] then B[D3,D23]=B[D3-D19,D23]+C[D23]:B1[D3,D23]=B1[D3-D19,D23] else B[D3,D23]=B[D3-D19,D23]
   next
   D3=D3+1
  next
  for D24=1 to D3
   for D25=1 to D2
    B1[D24,D25]=0
   next
  next
  D8=0:D9=0:D19=0
  for D26=1 to D2
   C[D26]=0
  next
 next
next
print A
