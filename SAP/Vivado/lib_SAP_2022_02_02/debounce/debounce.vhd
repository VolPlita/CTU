library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

entity debounce is
    Generic (width: integer := 22); -- 100 MHz clock -> 4 - simulation (90 ns); 22 - synthesis (ca. 20 ms)
    Port ( clk : in STD_LOGIC;
           tl_in : in STD_LOGIC;
           tl_out : out STD_LOGIC);
end debounce;

architecture Behavioral of debounce is

signal cnt       : std_logic_vector(width-1 downto 0) := (others => '0');
signal reset, ce : std_logic := '0';
signal tl_prev   : std_logic := '0';


begin

    counter: process(clk) begin
        if rising_edge(clk) then 
            if (reset = '1') then
                cnt <= (others => '0');
            elsif (ce = '1') then
                cnt <= cnt + 1;
            end if;
        end if;
    end process;

    process(clk) begin
        if rising_edge(clk) then
            if (tl_prev /= tl_in) then
                reset <= '1';
            else 
                reset <= '0';
            end if;
        end if;
    end process;
                
    process(clk) begin
        if rising_edge(clk) then
            if (reset = '1') then
                tl_prev <= tl_in;
            end if;
        end if;
    end process;
                
    process(clk) begin
        if rising_edge(clk) then
            if ((cnt(width-1) = '1') and (cnt(0) = '0')) then
                tl_out <= tl_prev;
            end if;
        end if;
    end process;
                

    ce <= '0' when ((cnt(width-1) = '1') and (cnt(0) = '1'))
                else '1';

end Behavioral;
