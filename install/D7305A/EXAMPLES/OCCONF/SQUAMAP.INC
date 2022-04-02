-- Mapping of osquare onto an n*n square array, supported by host
NODE Hostpass.p:
[p][p]NODE Multpass.p:
MAPPING
  DO
    MAP Hostpass.p ONTO HostSquare
    DO i = 0 FOR p
      DO j = 0 FOR p
        MAP Multpass.p[i][j] ONTO Square[i][j]
:
