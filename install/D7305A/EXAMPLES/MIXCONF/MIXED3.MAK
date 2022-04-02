mixed3.btl: mixed3.cfb
	icollect mixed3.cfb $(FLAG)p mixed3.map

mixed3.cfb: mixed3.pgm mixed3o.lku mixed3c.lku
	occonf mixed3.pgm

mixed3o.lku: mixed3o.tco
	ilink mixed3o.tco hostio.lib convert.lib $(FLAG)f occama.lnk $(FLAG)mo mixed3o.mod

mixed3o.tco: mixed3o.occ
	oc mixed3o.occ

mixed3c.lku: mixed3c.tco
	ilink $(FLAG)f mixed3c.lnk $(FLAG)mo mixed3c.mod

mixed3c.tco: mixed3c.c
	icc mixed3c.c
