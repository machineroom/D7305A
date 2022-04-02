-- Network description of a B003 supported by a host T800
-- Note additiional connections above those provided on the board itself

VAL p IS 2:

NODE HostSquare:
[p][p]NODE Square:
ARC Hostlink:
NETWORK
  DO
    SET HostSquare (type, memsize := "T800", 1*M)
    CONNECT HOST TO HostSquare[link][0] WITH Hostlink
    -- Connections from host to B003:
    CONNECT HostSquare[link][2] TO Square[0][0][link][0]
    CONNECT HostSquare[link][3] TO Square[1][1][link][0]
    DO i = 0 FOR p
      DO j = 0 FOR p
        SET Square[i][j] (type, memsize := "T414", 256*K)
    -- Hardwired connections:
    CONNECT Square[0][0][link][2] TO Square[1][0][link][3]
    CONNECT Square[1][0][link][2] TO Square[1][1][link][3]
    CONNECT Square[1][1][link][2] TO Square[0][1][link][3]
    CONNECT Square[0][1][link][2] TO Square[0][0][link][3]
    -- Additional wires:
    CONNECT Square[0][0][link][1] TO Square[1][1][link][1]
    CONNECT Square[1][0][link][1] TO Square[0][1][link][1]
    CONNECT Square[1][0][link][0] TO Square[0][1][link][0]
:
