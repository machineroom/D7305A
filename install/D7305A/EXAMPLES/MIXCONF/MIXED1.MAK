mixed1.btl: mixed1.cfb
	icollect mixed1.cfb $(FLAG)p mixed1.map

mixed1.cfb: mixed1.pgm mixed1c.lku
	occonf mixed1.pgm

mixed1c.lku: mixed1c.tco
	ilink $(FLAG)f mixed1c.lnk $(FLAG)mo mixed1c.mod

mixed1c.tco: mixed1c.c
	icc mixed1c.c
