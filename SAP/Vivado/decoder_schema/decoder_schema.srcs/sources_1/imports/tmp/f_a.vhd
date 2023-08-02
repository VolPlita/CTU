library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity f_a_vhdl is
   port (
      A0  :  in  std_logic;
      B0  :  in  std_logic;
      C0  :  in  std_logic;
      D0  :  in  std_logic;
      f_a :  out std_logic
   );
end entity f_a_vhdl;

architecture Behavioral of f_a_vhdl is


      signal A0_1    :    std_logic;
      signal and_0_O :    std_logic;
      signal and_1_O :    std_logic;
      signal and_2_O :    std_logic;
      signal and_3_O :    std_logic;
      signal and_4_O :    std_logic;
      signal and_5_O :    std_logic;
      signal B0_1    :    std_logic;
      signal C0_1    :    std_logic;
      signal D0_1    :    std_logic;
      signal or_0_O  :    std_logic;
begin
	A0_1 <= A0;
	B0_1 <= B0;
	C0_1 <= C0;
	D0_1 <= D0;
	f_a <= or_0_O;


	-- and_0
	and_0_O <= not(A0_1) and not(C0_1);

	-- and_1
	and_1_O <= B0_1 and not(D0_1);

	-- and_2
	and_2_O <= B0_1 and C0_1;

	-- and_3
	and_3_O <= A0_1 and C0_1 and not(D0_1);

	-- and_4
	and_4_O <= not(B0_1) and not(C0_1) and D0_1;

	-- and_5
	and_5_O <= not(A0_1) and C0_1 and D0_1;

	-- or_0
	or_0_O <= and_0_O or and_1_O or and_2_O or and_3_O or and_4_O or and_5_O;

end Behavioral;
