CONFIG
  --{{{  software description
  CHAN OF SP app.in, app.out:
  PLACE app.in, app.out ON Hostlink:
  [string.length+1]CHAN OF LETTERS pipe:
  PAR
    PROCESSOR inout.p
      inout (app.in, app.out, pipe[string.length], pipe[0])
    PAR i = 0 FOR string.length
      PROCESSOR pipe.element.p[i]
	sort.element (pipe[i], pipe[i+1])
  --}}}
:
