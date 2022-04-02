-- -----
--
-- (c) Inmos 1990
--
-- Generic assembler module for the initialisation code used in the
-- boot from EPROM run from RAM case, this is the primary bootstrap
-- code, and performs the following functions :-
--
--     1/ Allocate workspace for this module, the network loader,
--        also calculate logical memstart which is defined as the 
--        value inserted in the ROM immediately after this module,
--        and out wptr which follows this.
--
--     2/ Reset the High and Low priority process queues.
--
--     3/ Clear the error flag and initialise the halt-on-error flag. 
--
--     4/ Initialise the hardware, link and event, channels, 
--        and the timer pointer queues.
--
--     5/ Start the low and high priority timers. 
--
--     6/ Initialise the parameters for, and call, the network loader.
--
--     7/ Initialise the parameters for the system initialisation code. 
--
--     8/ Construct parameters for overlaying 
--
--     9/ Switch to reserved stack, and overlay.
--
--     10/ Move invocation stack, switch, and
--         Transfer control to system initialisation code.
--
--
--    The workspace used by this procedure is mapped as follows
--
--        offset       Variable
--
--          0            Return address for the called routines.
--
--	    1            Net loader links
--          2            Net loader data in ROM
--          3            Net loader memory
--          4            Net loader constant buffer address
--          5            Net loader address of loader data block
--
--          6            NL data block INT network.data.pointer
--          7            NL data block INT next.address
--          8            NL data block INT entry.point
--          9            NL data block INT data.point
--          10           NL data block INT invocation.stack
--
--          11           Stored value of logical memstart.
--          12           Loop control variable used in memory initialisation.
--          13           Loop counter used in memory initialisation.
--          14           Network loader entry point.
--                       also used as base of reserved_block (detailed following)
--
--          ...		 120 bytes of loader buffer.
--
--    overlaying the last n words of the loader buffer we have (offsets are at different times)
--    this is a total of 11 words, and we record this in comment II (or son of comment)
--
--          0           Move source		-- Overlaying variables
--          1           Move destination
--          2           Move length
--
--          0           Return address for the SI code, and the entry address
--          
--          1           SI boot link in 
--          2           SI boot link out
--          3           SI code address
--          4           SI data address
--          5           SI external address
--          6           SI processor
--	    end word	Copy of invocation stack address
--
-- -----

#define T426_PROCESSOR_LOWER    30
#define T426_PROCESSOR_UPPER    39

#define PARITY_ERROR_REGISTER   2147483492      -- 7FFFFF64

--

#define    NL_return_address              0

#define    NL_links                       1
#define    NL_network_data                2
#define    NL_memory                      3
#define    NL_buffer_address              4
#define    NL_exported_data               5

#define    NLDB                           6
#define    NLDB_network_data_pointer      6
#define    NLDB_next_address              7
#define    NLDB_entry_point               8
#define    NLDB_data_point                9
#define    NLDB_invocation_stack          10

#define    logical_memstart               11
#define    loop_control                   12
#define    loop_count                     13
#define    loader_entry_point             14
#define    base_reserved_block            loader_entry_point
#define    primary_workspace              (loader_entry_point+1)

--

#define    MOVE_source                    0
#define    MOVE_destination               1
#define    MOVE_length                    2

#define    SI_return_address              3
#define    SI_routine_address             3

#define    SI_boot_link_in                4
#define    SI_boot_link_out               5
#define    SI_code_address                6
#define    SI_data_address                7
#define    SI_external_address            8
#define    SI_processor                   9
#define    invocation_stack_address       10

#define    reserved_words                 (invocation_stack_address+1)

--

#define    subroutine_workspace           48
#define    required_workspace             (subroutine_workspace+primary_workspace)

#define    channel_words_to_clear         11
#define    loader_buffer_size             120

--

    align
Overlaying_machine_initialisation_runram:
    global Overlaying_machine_initialisation_runram

--

descriptor Overlaying_machine_initialisation_runram "Overlaying_machine_initialisation_runram" occam_harness  required_workspace loader_buffer_size ""

-- -----
--
--    1/ Allocate workspace, for the called routines. 
--
--    This has changed, now it reads two words from 
--    the rom 0,1 1 is the base of my memory area at
--    the top of RAM, we adjust for subroutine_workspace
--    and use that, 0 is the logical memstart to be passed 
--    to the network loader.
--
-- -----

    ldc         subroutine_workspace            -- Allow some workspace
    
    ldc         end_of_module - here_1          -- Get address of ROM variables
    ldpi
here_1:
    
    ldnl        1                               -- Read my memory
    
    wsub                                        -- Add subroutine_workspace words to iy

    gajw					-- Setup workspace ptr

--

    ldc         end_of_module - here_2          -- Get address of ROM variables
    ldpi
here_2:
    ldnl        0				-- read logical_memstart
    stl         logical_memstart                -- store logical_memstart

-- -----
--
--    2/ Reset High and Low priority process queues.
--
-- -----

    mint                                -- NotProcess.p
    sthf
    mint                                -- NotProcess.p
    stlf

-- -----
--
--    3/ Clear the error flag and initialise halt on error flag.
--
--    Note this is for halt on error but it could be for continue
--    on error by changing sethalterr to clrhalterr.
--
--    Note this has the T8 fpu error initialisation also.
--
--    Also clear parity regs for the T426
--
-- -----

    testerr                            -- Clear error flag
    $FPUERRCLR                         -- Clear floating point error flag

    $INITERR                           -- Set or clear halt on error.

--

#ifdef PARITY_CHECKING

    lddevid                            -- can do this since this is safe on all processors
    ldc    T426_PROCESSOR_LOWER-1
    gt                                 -- Only on T426s at present
    cj     Noparityregs
    ldc    T426_PROCESSOR_UPPER+1
    lddevid
    gt
    cj     Noparityregs
    ldc    PARITY_ERROR_REGISTER       -- Clear parity register by reading it
    ldnl   0

Noparityregs:

#endif

-- -----
--
--    4/ Initialise hardware channels and timer queues.
--       This is done by setting the bottom eleven words of memory
--       to NotProcess.p.
--
-- -----

                                       -- Setup loop control variables
    ldc         0                      -- Word number
    stl         loop_control
    ldc         channel_words_to_clear -- Number of times to loop
    stl         loop_count

loop:
    mint                               -- NotProcess.p, value to be written

    ldl         loop_control           -- Word number
    mint                               -- Base of memory
    wsub                               -- Convert to an address

    stnl        0                      -- Perform write.

    ldlp        loop_control           -- Address of loop control variables
    ldc         end_loop - loop        -- Distance to loop back
    lend                               -- Perform loop for 'count' times
end_loop:

-- -----
--
--    5/ Start low and High priority timers.
--
-- -----

    ldc        0                       -- Initial value for both timers.
    sttimer

-- -----
--
--    6/ Initialise the parameters for, and call, the network loader.
--
--    On arrival into the network loader the workspace will 
--    be viewed as :-
--
--        offset       Variable                             value
--
--          0       Return address.
--          1       Address of  [4]CHAN OF ANY links       mint
--          2       address of  [4]BYTE network.data       calculated as block
--                                                         after network loader
--          3       address of  [4]BYTE memory             logical_memstart
--          4       value of    VAL INT Buffer.address     offset from memory to buffer after my workspace
--          5       Address of  [5]INT Exported.data       loader_data_block
--
--
--    The loader data block contains the following variables returned from the network loader
--    and these are all converted to absoulute values by the network loader APART FROM
--    the entry point as this is relative to different positions on a run from ROM or run from 
--    RAM system, while the netloader is common.
--
--          0       INT network.data.pointer
--          1       INT next.address
--          2       INT entry.point
--          3       INT data.point
--          4       INT invocation.stack
--
-- -----

    ldc         return_point - here_3  		-- Get and store return address
    ldpi
here_3:
    stl         NL_return_address

--

    mint                               		-- Store address of links
    stl         NL_links

--

    ldc         end_of_module - here_4 		-- Get address of network loader block
    ldpi
here_4:
    ldnlp       2				-- Ignore the word containing logical memstart
    stl         NL_network_data			-- Store this temporarily

    ldl         NL_network_data			-- Add 4 to block start to find 
    adc         4				-- network loader entry point. (SKIP netloader length)
    stl         loader_entry_point     		-- store it.

    ldl         NL_network_data			-- Read length of netloader, 16 bits in 32 bit word
    ldnl        0                    		-- OK whatever processor.

    ldl         loader_entry_point     		-- add to block entry to find 
    bsub                               		-- start of next block.

    stl         NL_network_data			-- Write it to parameter block

--

    ldl         logical_memstart		-- Store logical_memstart as memory pointer
    stl         NL_memory

--

    ldlp        primary_workspace               -- address of buffer
    ldl         logical_memstart		-- convert to a byte offset from logical_memstart
    diff
    stl         NL_buffer_address

--

    ldlp        NLDB				-- Store pointer for network loader data block
    stl         NL_exported_data

--

    ldl         loader_entry_point     		-- Finally perform call
    gcall

--

return_point:

    ajw         -4                     		-- Restore Workspace pointer
                                       		-- to its value prior to the call.

-- -----
--
--    7/ Initialise the stack frame for the later call to the system initialisation code
--       NOTE we are writing into our local copy of the stack, in our reserved area,
--       this will be copied to the real stack later after overlaying is done.
--
--    On arrival into the initialisation code the workspace will 
--    be viewed as below, however at this point we store the address 
--    of the routine in offset 0.
--
--        offset       Variable                             value
--
--          0       Return address/ routine address.
--          1       Address of  CHAN OF ANY boot.link.in       mint+4words
--          2       Address of  CHAN OF ANY boot.link.out      mint
--          3       value of    VAL INT code.address           logical_memstart
--          4       value of    VAL INT data.address           logical_memstart
--          5       value of    VAL INT external.address       dummy mint
--          6       address of  [processor.size]INT processor  NLDB_data_point
--
--    NOTE this is when workspace will have relocated to NLDB_invocation_stack
--
-- -----

    ldlp        primary_workspace               -- Calculate the base of our reserved block
    ldc         loader_buffer_size
    bsub
    stl         base_reserved_block

    ldl         NLDB_invocation_stack		-- Copy the invocation stack pointer 
    ldl         base_reserved_block		-- into my safe area.
    stnl        invocation_stack_address

--

    ldl          NLDB_entry_point		-- Adjust to be relative to memstart.
    ldl          logical_memstart
    bsub

    ldl          base_reserved_block
    stnl         SI_routine_address

--

    ldc          4         			-- Get and Store address of link0 in
    mint
    wsub
    ldl          base_reserved_block
    stnl         SI_boot_link_in

--

    mint					-- Get and Store address of link0 out
    ldl          base_reserved_block
    stnl         SI_boot_link_out

--

    ldl          logical_memstart		-- Store logical_memstart in code.address
    ldl          base_reserved_block
    stnl         SI_code_address

--

    ldl          logical_memstart		-- Store logical_memstart in data.address
    ldl          base_reserved_block
    stnl         SI_data_address

--

    mint					-- Store dummy mint.
    ldl          base_reserved_block
    stnl         SI_external_address

--

    ldl          NLDB_data_point                -- Use data point as base of
    ldl          base_reserved_block		-- processor table
    stnl         SI_processor

-- -----
--    
--    8/ Construct the parameters for the final loading stage move loop.
--
-- -----

    ldl         NLDB_network_data_pointer	-- Round up pointer to 4 byte boundary
    adc         3
    ldc         -4
    and

    ldl         NL_network_data 		-- Add to start to find final loading block
    bsub

    ldl         base_reserved_block
    stnl        MOVE_source

--

    ldl         NLDB_next_address		-- Get destination
    ldl         base_reserved_block
    stnl        MOVE_destination

-- -----
--    
--    9/ Switch stack, to my reserved block, and do
--       the overlaying.
--
-- -----

    ldl         base_reserved_block
    gajw
    
--  

overlay_loop:

    ldl         MOVE_source                     -- Get length
    lb
    stl         MOVE_length

    ldl         MOVE_source                     -- Update source ptr
    ldc         1
    bsub
    stl         MOVE_source

    ldl         MOVE_length                     -- Check for any data to transfer
    cj          no_more_overlaying

    ldl         MOVE_source                     -- perform overlay move
    ldl         MOVE_destination
    ldl         MOVE_length
    move

    ldl         MOVE_destination                -- update destination ptr
    ldl         MOVE_length
    bsub
    stl         MOVE_destination

    ldl         MOVE_source                     -- update source ptr
    ldl         MOVE_length
    bsub
    stl         MOVE_source

    j           overlay_loop                    -- Jump for next overlay record

--

no_more_overlaying:

-- -----
--
--    10/ Move the invocation stack to its final resting place.
--        switch stacks
--        Transfer control to system process.
--
-- -----

    ldlp        SI_return_address		-- Base of invocation stack copy (source)
    ldl         invocation_stack_address	-- Base of real  invocation stack (dest)
    ldc         (invocation_stack_address-SI_return_address) -- Num words
    bcnt					-- Num bytes (length)
    move

--

    ldl         invocation_stack_address	-- Switch to invocation stack
    gajw

--

    ldl         0				-- Routine address stored here

--

    ldc         return_point2 - here_5  	-- Get and store return address
    ldpi
here_5:
    stl         0

--

    gcall

--

return_point2:

-- -----
--
--    Should the program ever terminate then stop.
--
-- -----

    stopp

-- -----
--
--    Here we have the end of this block, we have an alligned label
--    that is used as the pointer to the next block, containing a 16 
--    bit word giving the length of the network loader. followed by
--    the network loader.
--
-- -----

    align

end_of_module:

