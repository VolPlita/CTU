library IEEE;
use IEEE.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity hodiny is
    port(
        clk_100MHz : in std_logic;
        reset      : in std_logic;
        mux_sel    : out std_logic
    );
end hodiny;

architecture behavioral of hodiny is

    signal tmp : std_logic_vector(18 downto 0);

    ATTRIBUTE X_INTERFACE_PARAMETER : STRING;
    ATTRIBUTE X_INTERFACE_PARAMETER OF reset: SIGNAL IS "XIL_INTERFACENAME Reset, POLARITY ACTIVE_HIGH";

begin

    process (clk_100MHz, reset)
    begin
        if (Reset='1') then
            tmp <= (others => '0');
        elsif rising_edge(clk_100MHz) then
            tmp <= tmp + 1;
        end if;
    end process;

    mux_sel <= tmp(18);

end;
