library IEEE;
use IEEE.std_logic_1164.all;

entity output_logic is
    port(
        a1 : in  std_logic;
        a2 : in  std_logic;
        b1 : in  std_logic;
        b2 : in  std_logic;
        y  : out std_logic
    );
end output_logic;

architecture behavioral of output_logic is

begin

    y <= (a1 and a2 and (not b1) and b2);

end;
