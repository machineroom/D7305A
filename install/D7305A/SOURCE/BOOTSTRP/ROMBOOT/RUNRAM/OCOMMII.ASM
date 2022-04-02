-- -----
--
--     (c) Copyright INMOS Limited 1990.
--
--     Comment module for the boot from ROM run from RAM modules,
--     NOTE this is a non executable comment bootstrap.
--     This is a dummy version with zeros in all fields
--
-- -----

    align

Overlaying_comment_runram_II:
    global  Overlaying_comment_runram_II

    byte    0, 0, 0, 0          -- 32 bit 0 at start to indicate this is start

    byte    #07, #42, #54, #52  -- ^g BTR to indicate this is header.

    byte    #1			-- Version byte

    byte    0                   -- Unrecoverable space ( bytes ) at top of memory
    byte    11			-- Unrecoverable workspace ( words ) at top of memory

-- -----
--
--    Ensure that module is word length.
--
-- -----

    align

