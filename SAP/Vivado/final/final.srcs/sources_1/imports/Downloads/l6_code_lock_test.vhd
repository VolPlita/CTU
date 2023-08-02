library STD;
use STD.textio.all;                     -- basic I/O

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.STD_LOGIC_ARITH.all;
use IEEE.STD_LOGIC_UNSIGNED.all;
use IEEE.std_logic_textio.all;

entity code_lock_test is
    generic(
        C_CODE          : string := "ABCB";
        C_TEST1         : string := "AABNNCBA";
        C_TEST2         : string := "AABNNCBA";
        C_TEST3         : string := "NNBBABCBCC";
        C_FSM_TYPE      : string := "MOORE";
        C_COUNTER_DELAY : integer := 4        -- pulse delay
    );
    port(
        Clock   : out std_logic;
        Reset   : out std_logic;

        A       : out std_logic;
        B       : out std_logic;
        C       : out std_logic;

        Lock    : in  std_logic
    );  
end code_lock_test;

architecture behavior of code_lock_test is

    signal rst  : std_logic := '0';
    signal clk  : std_logic := '1';

    signal Counter_value     : integer := 0;
    signal ABC               : std_logic_vector(2 downto 0) := B"000";

    constant CLK_PERIOD      : time := 10 ns;
    
    signal terminate         : boolean;
    signal mealy             : boolean  := false;
    signal moore             : boolean  := false;
                
    ATTRIBUTE X_INTERFACE_INFO : STRING;
    ATTRIBUTE X_INTERFACE_PARAMETER : STRING;
    ATTRIBUTE X_INTERFACE_PARAMETER OF Reset: SIGNAL IS "XIL_INTERFACENAME Reset, POLARITY ACTIVE_HIGH";
    ATTRIBUTE X_INTERFACE_INFO OF Reset: SIGNAL IS "xilinx.com:signal:reset:1.0 Reset RST";
    ATTRIBUTE X_INTERFACE_PARAMETER OF Clock: SIGNAL IS "XIL_INTERFACENAME Clock, ASSOCIATED_RESET Reset, FREQ_HZ 100000000, PHASE 0.000";
    ATTRIBUTE X_INTERFACE_INFO OF Clock: SIGNAL IS "xilinx.com:signal:clock:1.0 Clock CLK";
    ATTRIBUTE X_INTERFACE_PARAMETER OF Reset: SIGNAL IS "LAYERED_METADATA undef";

    function char2std(c : character) return std_logic_vector is
    begin
        if c = 'A' or c = 'a' then
            return B"001";
        elsif c = 'B' or c = 'b' then
            return B"010";
        elsif c = 'C' or c ='c' then
            return B"100";
        end if;
        return B"000";
    end function;

    function fill_code(str : STRING) return std_logic_vector is
        variable code   : STRING(str'range) := str; 
        variable result : std_logic_vector(11 downto 0);
    begin
        if str'length < 4 then
            return B"000_000_000_000";
        end if;
        result(2 downto 0)  := char2std(str(4));
        result(5 downto 3)  := char2std(str(3));
        result(8 downto 6)  := char2std(str(2));
        result(11 downto 9)  := char2std(str(1));
        return result;
    end function;

    procedure shift_compare(
        signal val      : in  std_logic_vector(2 downto 0); 
        constant code   : in  std_logic_vector(11 downto 0);
        variable detec  : inout  std_logic_vector(11 downto 0);
        variable locked : out std_logic
        ) is
    begin
        if val /= B"000" then
            detec  := detec(8 downto 0) & val;
            locked := '0';
            if detec = code then
                locked := '1';
            end if;
        end if;
    end shift_compare;

    procedure run_test(
        constant code       : in  string;
        constant test       : in  string;
        signal ABC          : inout  std_logic_vector(2 downto 0); 
        signal moore        : in boolean;
        signal mealy        : in boolean
        ) is
        variable file_line          : line;
        variable file_line2         : line;
        variable locked             : std_logic;
        variable code_std           : std_logic_vector(11 downto 0) := fill_code(code);
        variable detec_code         : std_logic_vector(11 downto 0) := B"000_000_000_000";
        variable detec_str          : string(test'range)            := (others => '-');
        variable k                  : integer;
    begin
        write(file_line, string'("** Test:  "));
        write(file_line, test);
        writeline(output, file_line);
        for I in test'range loop
            write(file_line, test(I));
            write(file_line, string'(" "));

            locked := '0';
            wait until rising_edge(clk); wait for 1 ns;
            ABC <= char2std(test(I));
            wait for CLK_PERIOD/2;
            shift_compare(ABC, code_std, detec_code, locked);

            if locked = '1' then
                k := code'length;
                for j in I downto 1 loop
                    if char2std(test(j)) > B"000" then
                        if k > 0 then
                            detec_str(j) := code(k);
                            k := k-1;
                        end if;
                    end if;
                end loop;
            end if;
            
            if mealy then
                write(file_line2, Lock);
                write(file_line2, string'(" "));
            end if;
            wait until rising_edge(clk); wait for 1 ns;
            ABC <= B"000";
            wait for CLK_PERIOD/2;
            if moore then
                write(file_line2, Lock);
                write(file_line2, string'(" "));
            end if;
        end loop;
        writeline(output, file_line);
        writeline(output, file_line2);
        for j in detec_str'range loop
            write(file_line, detec_str(j));
            write(file_line, string'(" "));
        end loop;        
        writeline(output, file_line);
    end run_test;

    procedure do_reset(
        signal rst      : inout  std_logic
        ) is
        variable file_line          : line;
    begin
        write(file_line, string'(""));
        writeline(output, file_line);

        rst <= '1';
        wait until rising_edge(clk);
        wait for 1 ns;
        rst <= '0';
        
        wait for 4 ns;

        wait until rising_edge(clk);
    end do_reset;

begin

    buzzer: process
        variable file_line    : line;
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

    Clock   <= clk;
    A       <= ABC(0);
    B       <= ABC(1);
    C       <= ABC(2);

    MOORE_G : if C_FSM_TYPE = "MOORE" or C_FSM_TYPE = "moore" or C_FSM_TYPE = "Moore" generate
    begin
        moore <= true;
    end generate;

    MEALY_G : if C_FSM_TYPE = "MEALY" or C_FSM_TYPE = "mealy" or C_FSM_TYPE = "Mealy" generate
    begin
        mealy <= true;
    end generate;


    stim_proc: process
        variable file_line          : line;
    begin
        terminate <= false;
        wait for 1 ps;
        
        assert C_COUNTER_DELAY > 3 report "### C_COUNTER_DELAY should be greater than 3! ###" severity failure;
        assert moore or mealy report "### C_FSM_TYPE should be set to MOORE or MEALY! ###" severity failure;
        assert C_CODE'length = 4 report "### Length of the C_CODE should be equal to 4! ###" severity failure;
        assert C_TEST1'length > 3 report "### Length of the C_TEST1 should be greater to 3! ###" severity failure;
        assert C_TEST2'length > 3 report "### Length of the C_TEST2 should be greater to 3! ###" severity failure;
        assert C_TEST3'length > 3 report "### Length of the C_TEST3 should be greater to 3! ###" severity failure;

        -- zacatek simulace
        write(file_line, string'("### Simulation start ###"));
        writeline(output, file_line); -- write to display    
        write(file_line, string'("FSM_TYPE: "));
        write(file_line, C_FSM_TYPE);
        writeline(output, file_line);
        
        do_reset(rst);
        run_test(C_CODE, C_CODE, ABC, moore, mealy);
        wait until rising_edge(clk);
        do_reset(rst);
        run_test(C_CODE, C_TEST1, ABC, moore, mealy);
        wait until rising_edge(clk);
        do_reset(rst);
        run_test(C_CODE, C_TEST2, ABC, moore, mealy);
        wait until rising_edge(clk);
        do_reset(rst);
        run_test(C_CODE, C_TEST3, ABC, moore, mealy);

        -- konec simulace
        write(file_line, string'(""));
        writeline(output, file_line);
        write(file_line, string'("### Simulation finished ###"));
        writeline(output, file_line);

        terminate <= true;

        wait;
    end process;

    Reset   <= rst;


end;
