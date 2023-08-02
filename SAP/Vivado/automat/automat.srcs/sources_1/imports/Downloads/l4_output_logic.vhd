-- IEEE library
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- input and output ports definition
entity output_logic is
  Port (	-- Do not delete any port even if you do not use it
    q0    : in  std_logic;
    q1    : in  std_logic;
    q2    : in  std_logic;
    INC   : in  std_logic;
    DIR   : in  std_logic;
    y0    : out std_logic;
    y1    : out std_logic;
    y2    : out std_logic
  );	-- Do not delete any port even if you do not use it
end output_logic;

-- description of the FSM output functions
architecture Behavioral of output_logic is
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
  
  y0 <= q0; -- Keep connected to '0' if you do not use it
  y1 <= q1; -- Keep connected to '0' if you do not use it
  y2 <= q2; -- Keep connected to '0' if you do not use it

end Behavioral;
