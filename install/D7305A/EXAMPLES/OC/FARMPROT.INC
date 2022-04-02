--{{{  Work packet             -- Work packet to be sent down pipe.
PROTOCOL WORKPACKET
  CASE
    pipepacket;INT;INT    -- iplanex,y;x,y(screen)
    init
    terminate.3
:
--}}}
--{{{  Result packet           -- Coordinate data and result sent up pipe.
PROTOCOL RESULTPACKET
  CASE
    result.data; INT;INT;BYTE
    terminate.4
:
--}}}
--{{{  Packet complete         -- protocol used in the pipe to request work
PROTOCOL PACKETCOMPLETE
  CASE
    packet.processed
:
--}}}
