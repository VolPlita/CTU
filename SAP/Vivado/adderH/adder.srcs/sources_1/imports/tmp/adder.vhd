library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity adder_vhdl is
   port (
      A1    :  in  std_logic;
      B1    :  in  std_logic;
      C_in  :  in  std_logic;
      C_out :  out std_logic;
      S1    :  out std_logic
   );
end entity adder_vhdl;

architecture Behavioral of adder_vhdl is


      signal and_0_O :    std_logic;
      signal and_1_O :    std_logic;
      signal I0_0_1  :    std_logic;
      signal I1_0_1  :    std_logic;
      signal I1_1_1  :    std_logic;
      signal or_0_O  :    std_logic;
      signal xor_0_O :    std_logic;
      signal xor_1_O :    std_logic;
begin
	I0_0_1 <= A1;
	I1_0_1 <= B1;
	I1_1_1 <= C_in;
	C_out <= or_0_O;
	S1 <= xor_1_O;


	-- and_0
	and_0_O <= xor_0_O and I1_1_1;

	-- and_1
	and_1_O <= I0_0_1 and I1_0_1;

	-- or_0
	or_0_O <= and_0_O or and_1_O;

	-- xor_0
	xor_0_O <= I0_0_1 xor I1_0_1;

	-- xor_1
	xor_1_O <= xor_0_O xor I1_1_1;

end Behavioral;
