library IEEE;
use IEEE.std_logic_1164.all;

entity next_state_logic is
    port(
        a1 : in  std_logic;
        a2 : in  std_logic;
        b1 : in  std_logic;
        b2 : in  std_logic;
        n1 : out std_logic;
        n2 : out std_logic
    );
end next_state_logic;

architecture behavioral of next_state_logic is
begin

    n1 <= ( (a1 and (not b2)) or (b1 and (not b2)) or ((not a1) and a2 and b1) );
    n2 <= ( (a2 and (not b1) and (not b2)) or ((not a1) and a2 and b1 and b2) or (a1 and (not a2) and (not b1) and b2) );

end;
