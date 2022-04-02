-- occonf.inc

-- Useful constants defined for the occam configurer

-- History
-- 1.00 24/11/92 CON Original version
-- 1.01 03/12/92 SRH Corrected ROUTER.ORDER value
-- 1.02 28/01/93 SRH Added HIGH and LOW VALs

--{{{  Memory sizes
VAL K IS 1024 :
VAL M IS K * K :
--}}}

--{{{  Routing attribute values
--{{{  routecost
VAL MIN.COST          IS       1 :
VAL MAX.COST          IS 1000000 :
VAL DEFAULT.COST      IS    1000 :
VAL INFINITE.COST     IS 1000001 :
--}}}
--{{{  tolerance
VAL ZERO.TOLERANCE    IS       0 :
VAL DEFAULT.TOLERANCE IS       1 :
VAL MAX.TOLERANCE     IS 1000000 :
--}}}
--}}}

--{{{  Ordering attribute values
--{{{  Order of Virtual routing processs
VAL MUXER.ORDER       IS  -10000 :
VAL ROUTER.ORDER      IS  -20000 :
--}}}
--}}}

--{{{  Priorities for use in MAP statements
VAL HIGH IS 0 :
VAL LOW  IS 1 :
--}}}
