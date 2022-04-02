mixed2.btl: mixed2.cfb
	icollect mixed2.cfb $(FLAG)p mixed2.map

mixed2.cfb: mixed2.pgm mixed2o.lku mixed2c.lku
	occonf mixed2.pgm

mixed2o.lku: mixed2o.tco
	ilink mixed2o.tco hostio.lib convert.lib $(FLAG)f occama.lnk $(FLAG)mo mixed2o.mod $(FLAG)ta $(FLAG)h

mixed2o.tco: mixed2o.occ
	oc mixed2o.occ $(FLAG)ta $(FLAG)h

mixed2c.lku: mixed2c.tco
	ilink $(FLAG)f mixed2c.lnk $(FLAG)mo mixed2c.mod

mixed2c.tco: mixed2c.c
	icc mixed2c.c
