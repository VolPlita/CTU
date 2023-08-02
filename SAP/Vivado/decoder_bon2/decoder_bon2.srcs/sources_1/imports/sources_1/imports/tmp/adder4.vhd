library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity adder4_vhdl is
   port (
      A0    :  in  std_logic;
      A1    :  in  std_logic;
      A2    :  in  std_logic;
      A3    :  in  std_logic;
      B0    :  in  std_logic;
      B1    :  in  std_logic;
      B2    :  in  std_logic;
      B3    :  in  std_logic;
      C_in  :  in  std_logic;
      C_out :  out std_logic;
      S0    :  out std_logic;
      S1    :  out std_logic;
      S2    :  out std_logic;
      S3    :  out std_logic
   );
end entity adder4_vhdl;

architecture Behavioral of adder4_vhdl is
	component adder_vhdl
		port (
			A1    :  in  std_logic;
			B1    :  in  std_logic;
			C_in  :  in  std_logic;
			C_out :  out std_logic;
			S1    :  out std_logic
		);
	end component;



      signal A1_0_1             :    std_logic;      
      signal A1_1_1             :    std_logic;      
      signal A1_2_1             :    std_logic;      
      signal A1_3_1             :    std_logic;      
      signal adder_vhdl_0_C_out :    std_logic;      
      signal adder_vhdl_0_S1    :    std_logic;      
      signal adder_vhdl_1_C_out :    std_logic;      
      signal adder_vhdl_1_S1    :    std_logic;      
      signal adder_vhdl_2_C_out :    std_logic;      
      signal adder_vhdl_2_S1    :    std_logic;      
      signal adder_vhdl_3_C_out :    std_logic;      
      signal adder_vhdl_3_S1    :    std_logic;      
      signal B1_0_1             :    std_logic;      
      signal B1_1_1             :    std_logic;      
      signal B1_2_1             :    std_logic;      
      signal B1_3_1             :    std_logic;      
      signal C_in_0_1           :    std_logic;      
begin
	A1_0_1 <= A0;
	A1_1_1 <= A1;
	A1_2_1 <= A2;
	A1_3_1 <= A3;
	B1_0_1 <= B0;
	B1_1_1 <= B1;
	B1_2_1 <= B2;
	B1_3_1 <= B3;
	C_in_0_1 <= C_in;
	C_out <= adder_vhdl_3_C_out;
	S0 <= adder_vhdl_0_S1;
	S1 <= adder_vhdl_1_S1;
	S2 <= adder_vhdl_2_S1;
	S3 <= adder_vhdl_3_S1;


	-- adder_vhdl_0
	inst_adder_vhdl_0 : adder_vhdl
		port map (
			A1    =>  A1_0_1,
			B1    =>  B1_0_1,
			C_in  =>  C_in_0_1,
			C_out =>  adder_vhdl_0_C_out,
			S1    =>  adder_vhdl_0_S1
		);

	-- adder_vhdl_1
	inst_adder_vhdl_1 : adder_vhdl
		port map (
			A1    =>  A1_1_1,
			B1    =>  B1_1_1,
			C_in  =>  adder_vhdl_0_C_out,
			C_out =>  adder_vhdl_1_C_out,
			S1    =>  adder_vhdl_1_S1
		);

	-- adder_vhdl_2
	inst_adder_vhdl_2 : adder_vhdl
		port map (
			A1    =>  A1_2_1,
			B1    =>  B1_2_1,
			C_in  =>  adder_vhdl_1_C_out,
			C_out =>  adder_vhdl_2_C_out,
			S1    =>  adder_vhdl_2_S1
		);

	-- adder_vhdl_3
	inst_adder_vhdl_3 : adder_vhdl
		port map (
			A1    =>  A1_3_1,
			B1    =>  B1_3_1,
			C_in  =>  adder_vhdl_2_C_out,
			C_out =>  adder_vhdl_3_C_out,
			S1    =>  adder_vhdl_3_S1
		);

end Behavioral;
