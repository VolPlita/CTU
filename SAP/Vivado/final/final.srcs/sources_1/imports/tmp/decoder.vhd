library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity decoder_vhdl is
   port (
      A   :  in  std_logic;
      B   :  in  std_logic;
      C   :  in  std_logic;
      ERR :  out std_logic;
      Y1  :  out std_logic;
      Y2  :  out std_logic
   );
end entity decoder_vhdl;

architecture Behavioral of decoder_vhdl is


      signal A_1      :    std_logic;
      signal and_ab_O :    std_logic;
      signal and_ac_O :    std_logic;
      signal and_bc_O :    std_logic;
      signal B_1      :    std_logic;
      signal C_1      :    std_logic;
      signal or_err_O :    std_logic;
      signal or_y1_O  :    std_logic;
      signal or_y2_O  :    std_logic;
begin
	A_1 <= A;
	B_1 <= B;
	C_1 <= C;
	ERR <= or_err_O;
	Y1 <= or_y1_O;
	Y2 <= or_y2_O;


	-- and_ab
	and_ab_O <= A_1 and B_1;

	-- and_ac
	and_ac_O <= A_1 and C_1;

	-- and_bc
	and_bc_O <= B_1 and C_1;

	-- or_err
	or_err_O <= and_ab_O or and_bc_O or and_ac_O;

	-- or_y1
	or_y1_O <= B_1 or C_1;

	-- or_y2
	or_y2_O <= A_1 or C_1;

end Behavioral;
