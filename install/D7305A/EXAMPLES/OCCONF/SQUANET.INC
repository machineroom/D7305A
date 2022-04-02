-- Any p*p network of T425's set up for osquare or ozigzag

VAL Up IS 0:
VAL Left IS 1:
VAL Down IS 2:
VAL Right IS 3:
NODE HostSquare:
[p][p]NODE Square:
ARC Hostlink:
NETWORK
  DO
    SET HostSquare (type, memsize := "T425", 1*M)
    CONNECT HOST TO HostSquare[link][0] WITH Hostlink
    CONNECT HostSquare[link][3] TO Square[p-1][p-1][link][Down]
    DO i = 0 FOR p
      DO j = 0 FOR p
        SET Square[i][j] (type, memsize := "T425", 1*M)
    --{{{  Up and down
    DO i = 0 FOR p
      DO j = 0 FOR p
        IF
	  (i = 0) AND (j = 0)
	    CONNECT HostSquare[link][2] TO Square[0][0][link][Up]
	  i = 0
	    CONNECT Square[p - 1][j - 1][link][Down] TO Square[0][j][link][Up]
	  TRUE
	    CONNECT Square[i - 1][j][link][Down] TO Square[i][j][link][Up]
    --}}}  
    --{{{  Left and right
    DO i = 0 FOR p
      DO j = 0 FOR p
        IF
	  j = (p-1)
	    CONNECT Square[i][j][link][Right] TO Square[(i + 1)\p][0][link][Left]
	  TRUE
	    CONNECT Square[i][j][link][Right] TO Square[i][j + 1][link][Left]
    --}}}  
:
