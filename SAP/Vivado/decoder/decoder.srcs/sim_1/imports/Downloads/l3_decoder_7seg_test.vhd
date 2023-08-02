library STD;
use STD.textio.all;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_textio.all;

entity decoder_7seg_test is
end decoder_7seg_test;

architecture behavioral of decoder_7seg_test is

    signal input : std_logic_vector (3 downto 0);

    signal decoder_output : std_logic_vector (6 downto 0);

    signal no_of_errors : integer := 0;
    constant delay : time := 60 ns;

    type correct_value_type is array (0 to 15) of std_logic_vector(6 downto 0);

    constant correct_value : correct_value_type := (
        "1000000", -- 0
        "1111001", -- 1
        "0100100", -- 2
        "0110000", -- 3
        "0011001", -- 4
        "0010010", -- 5
        "0000010", -- 6
        "1111000", -- 7
        "0000000", -- 8
        "0010000", -- 9
        "0001000", -- a
        "0000011", -- b
        "1000110", -- c
        "0100001", -- d
        "0000110", -- e
        "0001110"  -- f
    );

    -- poznamka pro cvicici: deklarace komponenty neni nutna - vyber testovaneho obvodu je az u konkretni instance

begin

    -- definition of tested entity and its inputs and outputs
    -- inputs:  a, b, c, d mapped to input signal
    -- outputs: f_a, f_b, f_c, f_d, f_e, f_f, f_g mapped to decoder_output signal
    -- ATTENTION!
    -- The entity name (entity work.decoder_7seg) must be equal to the tested component name
    -- In this case: decoder_7seg
    -- The input and output names (left part before arrow) must be equal to the names in the tested design

    -- definice vstupu a vystupu testovane komponenty
    -- vstupy:  a, b, c, d pripojene na signal input
    -- vystupy: f_a, f_b, f_c, f_d, f_e, f_f, f_g pripojene na signal decoder_output
    -- POZOR!
    -- nazev entity (entity work.decoder_7seg) musi byt stejny jako nazev testovane entity
    -- v tomto pripade: decoder_7seg
    -- vstupy a vystupy (v leve casti pred sipkou) musi byt stejne jako popisy vstupu a vystupu testovane entity

    uut: entity work.decoder_7seg
    port map (
        d => input(3),
        c => input(2),
        b => input(1),
        a => input(0),
        f_a => decoder_output(0),
        f_b => decoder_output(1),
        f_c => decoder_output(2),
        f_d => decoder_output(3),
        f_e => decoder_output(4),
        f_f => decoder_output(5),
        f_g => decoder_output(6)
    );

    stim_proc: process
        variable file_line : line;
    begin

        write(file_line, string'("### Simulation start ###"));
        writeline(output, file_line);

        for i in 0 to 15 loop
            input <= conv_std_logic_vector(i, 4);
            wait for 1 ns;
            if (correct_value(i) /= decoder_output) then
                write(file_line, string'("Error in displaying "));
                hwrite(file_line, input); -- hex output
                write(file_line, string'(" / Chyba pri zobrazeni "));
                hwrite(file_line, input); -- hex output
                writeline(output, file_line);
                no_of_errors <= no_of_errors + 1;
            end if;

            wait for DELAY;
        end loop;

        -- end of simulation
        write(file_line, string'("### Total number of errors = "));
        write(file_line, no_of_errors);
        writeline(output, file_line);

        write(file_line, string'("### Simulation finished ###"));
        writeline(output, file_line);

        wait;
    end process;

end;
