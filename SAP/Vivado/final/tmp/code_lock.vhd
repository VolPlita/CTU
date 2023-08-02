library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity code_lock_vhdl is
   port (
      BTN_A :  in  std_logic;
      BTN_B :  in  std_logic;
      BTN_C :  in  std_logic;
      CLK   :  in  std_logic;
      y0    :  out std_logic
   );
end entity code_lock_vhdl;

architecture Behavioral of code_lock_vhdl is
	component automat_vhdl
		port (
			b1    :  in  std_logic;
			b2    :  in  std_logic;
			CLK   :  in  std_logic;
			RESET :  in  std_logic;
			y0    :  out std_logic
		);
	end component;

	component button_vhdl
		port (
			BTN   :  in  std_logic;
			CLK   :  in  std_logic;
			reset :  in  std_logic;
			O1    :  out std_logic
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



      signal automat_vhdl_0_y0  :    std_logic;      
      signal BTN_0_1            :    std_logic;      
      signal BTN_1_1            :    std_logic;      
      signal BTN_2_1            :    std_logic;      
      signal button_A_O1        :    std_logic;      
      signal button_B_O1        :    std_logic;      
      signal button_C_O1        :    std_logic;      
      signal CLK_1              :    std_logic;      
      signal decoder_vhdl_0_ERR :    std_logic;      
      signal decoder_vhdl_0_Y1  :    std_logic;      
      signal decoder_vhdl_0_Y2  :    std_logic;      
      signal gnd_0_O            :    std_logic;      
begin
	BTN_2_1 <= BTN_A;
	BTN_0_1 <= BTN_B;
	BTN_1_1 <= BTN_C;
	CLK_1 <= CLK;
	y0 <= automat_vhdl_0_y0;


	-- automat_vhdl_0
	inst_automat_vhdl_0 : automat_vhdl
		port map (
			b1    =>  decoder_vhdl_0_Y1,
			b2    =>  decoder_vhdl_0_Y2,
			CLK   =>  CLK_1,
			RESET =>  decoder_vhdl_0_ERR,
			y0    =>  automat_vhdl_0_y0
		);

	-- button_A
	inst_button_A : button_vhdl
		port map (
			BTN   =>  BTN_2_1,
			CLK   =>  CLK_1,
			reset =>  gnd_0_O,
			O1    =>  button_A_O1
		);

	-- button_B
	inst_button_B : button_vhdl
		port map (
			BTN   =>  BTN_0_1,
			CLK   =>  CLK_1,
			reset =>  gnd_0_O,
			O1    =>  button_B_O1
		);

	-- button_C
	inst_button_C : button_vhdl
		port map (
			BTN   =>  BTN_1_1,
			CLK   =>  CLK_1,
			reset =>  gnd_0_O,
			O1    =>  button_C_O1
		);

	-- decoder_vhdl_0
	inst_decoder_vhdl_0 : decoder_vhdl
		port map (
			A   =>  button_A_O1,
			B   =>  button_B_O1,
			C   =>  button_C_O1,
			ERR =>  decoder_vhdl_0_ERR,
			Y1  =>  decoder_vhdl_0_Y1,
			Y2  =>  decoder_vhdl_0_Y2
		);

	-- gnd_0
	gnd_0_O <= '0';

end Behavioral;
