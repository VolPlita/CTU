-- IEEE library
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- input and output ports definition
entity next_state_logic is
  Port (	-- Do not delete any port even if you do not use it
    q0    : in  std_logic;
    q1    : in  std_logic;
    q2    : in  std_logic;
    INC   : in  std_logic;
    DIR   : in  std_logic;
    d0    : out std_logic;
    d1    : out std_logic;
    d2    : out std_logic
  );	-- Do not delete any port even if you do not use it
end next_state_logic;

-- description of the FSM transition functions
architecture Behavioral of next_state_logic is
begin
  -- use the following operators in all expressions:
  -- ------------------------------------
  -- | operator | example               |
  -- -----------------------------------|
  -- |    and   |	a and b and c       |
  -- |    or    |	a or b or (c and d) |
  -- |    not   | not a                 |
  -- ------------------------------------
  -- use brackets!
  -- example:
  --   assignment:                            f(a, b, c) = a * #b * #c + a * b
  --   representation: f <= (a and (not b) and (not c)) or (a and b) 
  
  d0 <= ( (q0 and (not INC)) or ((not q0) and INC) ); -- Keep connected to '0' if you do not use it
  d1 <= ( (q1 and (not INC)) or (q1 and (not q0)) or (q0 and (not q1) and (not q2) and INC) ); -- Keep connected to '0' if you do not use it
  d2 <= ( (q2 and (not INC)) or ((not q0) and q2) or (q0 and q1 and INC) ); -- Keep connected to '0' if you do not use it

end Behavioral;
