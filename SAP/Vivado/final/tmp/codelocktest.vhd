library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity codelocktest_vhdl is
   port (
      Reset_0 :  out std_logic
   );
end entity codelocktest_vhdl;

architecture Behavioral of codelocktest_vhdl is
	component automat_vhdl
		port (
			b1    :  in  std_logic;
			b2    :  in  std_logic;
			CLK   :  in  std_logic;
			RESET :  in  std_logic;
			y0    :  out std_logic
		);
	end component;

	component code_lock_test
		port (
			Lock  :  in  std_logic;
			A     :  out std_logic;
			B     :  out std_logic;
			C     :  out std_logic;
			Clock :  out std_logic;
			Reset :  out std_logic
		);
	end component;

	component decoder_vhdl
		port (
			A   :  in  std_logic;
			B   :  in  std_logic;
			C   :  in  std_logic;
			ERR :  out std_logic;
			Y1  :  out std_logic;
			Y2  :  out std_logic
		);
	end component;



      signal automat_vhdl_0_y0      :    std_logic;          
      signal code_lock_test_0_A     :    std_logic;          
      signal code_lock_test_0_B     :    std_logic;          
      signal code_lock_test_0_C     :    std_logic;          
      signal code_lock_test_0_Clock :    std_logic;          
      signal code_lock_test_0_Reset :    std_logic;          
      signal decoder_vhdl_0_ERR     :    std_logic;          
      signal decoder_vhdl_0_Y1      :    std_logic;          
      signal decoder_vhdl_0_Y2      :    std_logic;          
begin
	Reset_0 <= code_lock_test_0_Reset;


	-- automat_vhdl_0
	inst_automat_vhdl_0 : automat_vhdl
		port map (
			b1    =>  decoder_vhdl_0_Y1,
			b2    =>  decoder_vhdl_0_Y2,
			CLK   =>  code_lock_test_0_Clock,
			RESET =>  decoder_vhdl_0_ERR,
			y0    =>  automat_vhdl_0_y0
		);

	-- code_lock_test_0
	inst_code_lock_test_0 : code_lock_test
		port map (
			Lock  =>  automat_vhdl_0_y0,
			A     =>  code_lock_test_0_A,
			B     =>  code_lock_test_0_B,
			C     =>  code_lock_test_0_C,
			Clock =>  code_lock_test_0_Clock,
			Reset =>  code_lock_test_0_Reset
		);

	-- decoder_vhdl_0
	inst_decoder_vhdl_0 : decoder_vhdl
		port map (
			A   =>  code_lock_test_0_A,
			B   =>  code_lock_test_0_B,
			C   =>  code_lock_test_0_C,
			ERR =>  decoder_vhdl_0_ERR,
			Y1  =>  decoder_vhdl_0_Y1,
			Y2  =>  decoder_vhdl_0_Y2
		);

end Behavioral;
