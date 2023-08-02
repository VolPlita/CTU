library STD;
use STD.textio.all;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_textio.all;
use IEEE.std_logic_unsigned.all;

entity automat_test is
    generic(
        C_MODULO   : natural := 7;       -- counter mod <C_MODULO> (max 8)
        C_FSM_TYPE : string  := "MOORE"; -- counter type: MEALY or MOORE
        C_OUT_TYPE : string  := "GRAY";  -- counter output type: BINARY or GRAY
        C_DIR      : boolean := False    -- True, if the FSM have an input determining the direction. False otherwise
    );
end automat_test;

architecture behavioral of automat_test is

    signal d   : std_logic_vector(2 downto 0) := "000";
    signal q   : std_logic_vector(2 downto 0) := "000";
    signal dir : std_logic := '1';
    signal inc : std_logic := '0';

    signal reset : std_logic := '0';
    signal clk   : std_logic := '1';

    signal y : std_logic_vector(2 downto 0);

    signal no_of_errors : integer := 0;
    signal terminate    : boolean;


    function calc_period(modulo : natural; dir : boolean) return time is
    begin
        if dir then
            return((950 ns) / (2*(2*(modulo + 1) + 3)));
        else
            return((950 ns) / (2*(modulo + 1) + 3));
        end if;
    end function calc_period;

    constant CLK_PERIOD : time := calc_period(C_MODULO, C_DIR);

    procedure print_check(
        inc : std_logic;

        d : std_logic_vector(2 downto 0);
        y : std_logic_vector(2 downto 0);
        c : std_logic_vector(2 downto 0);

        signal no_of_errors : inout integer
    ) is
    variable file_line : line;
    begin
        write(file_line, string'("  INC = "));
        write(file_line, inc);

        write(file_line, string'(" Next = 0x"));
        hwrite(file_line, d);
        write(file_line, string'(" Output = 0x"));
        hwrite(file_line, y);
        write(file_line, string'(" Correct = 0x"));
        hwrite(file_line, c);
        if y /= c then
            write(file_line, string'(" - ERROR"));
            no_of_errors <= no_of_errors + 1;
        else
            write(file_line, string'(" - OK"));
        end if;
        writeline(output, file_line);
    end;

begin

    -- definition of tested entities and theirs inputs and outputs
    -- inputs:  q0, q1, q2, inc, dir
    -- outputs: d0, d1, d2 and y0, y1, y2
    -- ATTENTION!
    -- The entities names (entity work.next_state_logic and entity work.output_logic) must be equal to the tested component names
    -- In this case: next_state_logic and output_logic
    -- The input and output names (left part before arrow) must be equal to the names in the tested designs

    next_state_logic_uut: entity work.next_state_logic
    port map(
        q0  => q(0),
        q1  => q(1),
        q2  => q(2),
        inc => inc,
        dir => dir,
        d0  => d(0),
        d1  => d(1),
        d2  => d(2)
      );

    output_logic_uut: entity work.output_logic
    port map(
        q0  => q(0),
        q1  => q(1),
        q2  => q(2),
        inc => inc,
        dir => dir,
        y0  => y(0),
        y1  => y(1),
        y2  => y(2)
    );

    buzzer: process
        variable file_line : line;
    begin
        wait for 1000 ns;
        if (not terminate) then
            write(file_line, string'("-->>> Simulation paused - press Run-all to continue! <<<--"));
            writeline(output, file_line);
        end if;
        wait;
    end process;

    process
    begin
        clk <= not clk;
        wait for CLK_PERIOD/2;
        if (terminate) then
            wait;
        end if;
    end process;

    process (clk, reset)
    begin
        if reset = '1' then
            q <= (others => '0');
        elsif (clk'event and clk = '1') then
            q <= d;
        end if;
    end process;

    stim_proc: process
        type t_data is array (0 to 7) of std_logic_vector(2 downto 0);
        constant binary: t_data := (
            "000", "001", "010", "011", "100", "101", "110", "111"
        );
        constant gray: t_data := (
            "000", "001", "011", "010", "110", "111", "101", "100"
        );
        variable j         : natural range 0 to 7 := 0;
        variable file_line : line;
    begin

        write(file_line, string'("### Simulation start ###"));
        writeline(output, file_line);

        reset <= '1';
        wait for 3*CLK_PERIOD;
        wait for 1 ns;
        reset <= '0';

        -- dir = 1 -> increment
        if C_DIR then
            write(file_line, string'("DIR = 1"));
            writeline(output, file_line);
        end if;

        for i in 0 to C_MODULO loop
            j := i mod C_MODULO;

            write(file_line, string'(" State = 0x"));
            hwrite(file_line, q);
            writeline(output, file_line);

            wait for 1 ns;
            if C_OUT_TYPE = "GRAY" then
                print_check(inc, d, y, gray(j), no_of_errors);
            else -- BINARY
                print_check(inc, d, y, binary(j), no_of_errors);
            end if;

            wait until rising_edge(clk);
            wait for 1 ns;
            inc <= '1';

            wait for 1 ns;
            if C_FSM_TYPE = "MEALY" then
                j := (i+1) mod C_MODULO;
            end if;

            if C_OUT_TYPE = "GRAY" then
                print_check(inc, d, y, gray(j), no_of_errors);
            else -- BINARY
                print_check(inc, d, y, binary(j), no_of_errors);
            end if;

            wait until rising_edge(clk);
            wait for 1 ns;
            inc <= '0';
        end loop;

        inc <= '0';

        -- dir = 0 -> decrement
        if C_DIR then

            reset <= '1';
            wait for 3*CLK_PERIOD;
            wait for 1 ns;
            reset <= '0';

            dir <= '0';
            write(file_line, string'("DIR = 0"));
            writeline(output, file_line);

            for i in C_MODULO downto 0 loop
                j := i mod C_MODULO;

                write(file_line, string'(" State = 0x"));
                hwrite(file_line, q);
                writeline(output, file_line);

                wait for 1 ns;
                if C_OUT_TYPE = "GRAY" then
                    print_check(inc, d, y, gray(j), no_of_errors);
                else -- BINARY
                    print_check(inc, d, y, binary(j), no_of_errors);
                end if;

                wait until rising_edge(clk);
                wait for 1 ns;
                inc <= '1';

                wait for 1 ns;
                if C_FSM_TYPE = "MEALY" then
                    j := (i-1) mod C_MODULO;
                end if;

                if C_OUT_TYPE = "GRAY" then
                    print_check(inc, d, y, gray(j), no_of_errors);
                else -- BINARY
                    print_check(inc, d, y, binary(j), no_of_errors);
                end if;

                wait until rising_edge(clk);
                wait for 1 ns;
                inc <= '0';
            end loop;
        end if;

        -- end of simulation
        write(file_line, string'("### Total number of errors = "));
        write(file_line, no_of_errors);
        writeline(output, file_line);

        write(file_line, string'("### Simulation finished ###"));
        writeline(output, file_line);

        terminate <= true;

        wait;

    end process;

end;
