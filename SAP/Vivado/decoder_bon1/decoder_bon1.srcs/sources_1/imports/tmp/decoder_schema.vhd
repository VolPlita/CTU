library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity decoder_schema_vhdl is
   port (
      BNTR :  in  std_logic;
      SW0  :  in  std_logic;
      SW1  :  in  std_logic;
      SW2  :  in  std_logic;
      SW3  :  in  std_logic;
      SW4  :  in  std_logic;
      SW5  :  in  std_logic;
      SW6  :  in  std_logic;
      SW7  :  in  std_logic;
      W5   :  in  std_logic;
      AN0  :  out std_logic;
      AN1  :  out std_logic;
      AN2  :  out std_logic;
      AN3  :  out std_logic;
      CA   :  out std_logic;
      CB   :  out std_logic;
      CC   :  out std_logic;
      CD   :  out std_logic;
      CE   :  out std_logic;
      CF   :  out std_logic;
      CG   :  out std_logic
   );
end entity decoder_schema_vhdl;

architecture Behavioral of decoder_schema_vhdl is
	component adder4_vhdl
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
	end component;

	component decoder_7seg
		port (
			a   :  in  std_logic;
			b   :  in  std_logic;
			c   :  in  std_logic;
			d   :  in  std_logic;
			f_a :  out std_logic;
			f_b :  out std_logic;
			f_c :  out std_logic;
			f_d :  out std_logic;
			f_e :  out std_logic;
			f_f :  out std_logic;
			f_g :  out std_logic
		);
	end component;

	component hodiny
		port (
			clk_100MHz :  in  std_logic;
			reset      :  in  std_logic;
			mux_sel    :  out std_logic
		);
	end component;



      signal adder4_vhdl_0_C_out :    std_logic;       
      signal adder4_vhdl_0_S0    :    std_logic;       
      signal adder4_vhdl_0_S1    :    std_logic;       
      signal adder4_vhdl_0_S2    :    std_logic;       
      signal adder4_vhdl_0_S3    :    std_logic;       
      signal BNTR_1              :    std_logic;       
      signal clk_100MHz_0_1      :    std_logic;       
      signal decoder_7seg_0_f_a  :    std_logic;       
      signal decoder_7seg_0_f_b  :    std_logic;       
      signal decoder_7seg_0_f_c  :    std_logic;       
      signal decoder_7seg_0_f_d  :    std_logic;       
      signal decoder_7seg_0_f_e  :    std_logic;       
      signal decoder_7seg_0_f_f  :    std_logic;       
      signal decoder_7seg_0_f_g  :    std_logic;       
      signal decoder_7seg_1_f_a  :    std_logic;       
      signal decoder_7seg_1_f_b  :    std_logic;       
      signal decoder_7seg_1_f_c  :    std_logic;       
      signal decoder_7seg_1_f_d  :    std_logic;       
      signal decoder_7seg_1_f_e  :    std_logic;       
      signal decoder_7seg_1_f_f  :    std_logic;       
      signal decoder_7seg_1_f_g  :    std_logic;       
      signal gnd_2_O             :    std_logic;       
      signal hodiny_0_mux_sel    :    std_logic;       
      signal inv_0_O             :    std_logic;       
      signal mux_0_O_0           :    std_logic;       
      signal mux_0_O_1           :    std_logic;       
      signal mux_0_O_2           :    std_logic;       
      signal mux_0_O_3           :    std_logic;       
      signal mux_0_O_4           :    std_logic;       
      signal mux_0_O_5           :    std_logic;       
      signal mux_0_O_6           :    std_logic;       
      signal SW0_1               :    std_logic;       
      signal SW1_1               :    std_logic;       
      signal SW2_1               :    std_logic;       
      signal SW3_1               :    std_logic;       
      signal SW4_1               :    std_logic;       
      signal SW5_1               :    std_logic;       
      signal SW6_1               :    std_logic;       
      signal SW7_1               :    std_logic;       
      signal vcc_1_O             :    std_logic;       
      signal vcc_2_O             :    std_logic;       
begin
	BNTR_1 <= BNTR;
	SW0_1 <= SW0;
	SW1_1 <= SW1;
	SW2_1 <= SW2;
	SW3_1 <= SW3;
	SW4_1 <= SW4;
	SW5_1 <= SW5;
	SW6_1 <= SW6;
	SW7_1 <= SW7;
	clk_100MHz_0_1 <= W5;
	AN0 <= inv_0_O;
	AN1 <= hodiny_0_mux_sel;
	AN2 <= vcc_1_O;
	AN3 <= vcc_2_O;
	CA <= mux_0_O_0;
	CB <= mux_0_O_1;
	CC <= mux_0_O_2;
	CD <= mux_0_O_3;
	CE <= mux_0_O_4;
	CF <= mux_0_O_5;
	CG <= mux_0_O_6;


	-- adder4_vhdl_0
	inst_adder4_vhdl_0 : adder4_vhdl
		port map (
			A0    =>  SW0_1,
			A1    =>  SW1_1,
			A2    =>  SW2_1,
			A3    =>  SW3_1,
			B0    =>  SW4_1,
			B1    =>  SW5_1,
			B2    =>  SW6_1,
			B3    =>  SW7_1,
			C_in  =>  BNTR_1,
			C_out =>  adder4_vhdl_0_C_out,
			S0    =>  adder4_vhdl_0_S0,
			S1    =>  adder4_vhdl_0_S1,
			S2    =>  adder4_vhdl_0_S2,
			S3    =>  adder4_vhdl_0_S3
		);

	-- decoder_7seg_0
	inst_decoder_7seg_0 : decoder_7seg
		port map (
			a   =>  adder4_vhdl_0_C_out,
			b   =>  gnd_2_O,
			c   =>  gnd_2_O,
			d   =>  gnd_2_O,
			f_a =>  decoder_7seg_0_f_a,
			f_b =>  decoder_7seg_0_f_b,
			f_c =>  decoder_7seg_0_f_c,
			f_d =>  decoder_7seg_0_f_d,
			f_e =>  decoder_7seg_0_f_e,
			f_f =>  decoder_7seg_0_f_f,
			f_g =>  decoder_7seg_0_f_g
		);

	-- decoder_7seg_1
	inst_decoder_7seg_1 : decoder_7seg
		port map (
			a   =>  adder4_vhdl_0_S0,
			b   =>  adder4_vhdl_0_S1,
			c   =>  adder4_vhdl_0_S2,
			d   =>  adder4_vhdl_0_S3,
			f_a =>  decoder_7seg_1_f_a,
			f_b =>  decoder_7seg_1_f_b,
			f_c =>  decoder_7seg_1_f_c,
			f_d =>  decoder_7seg_1_f_d,
			f_e =>  decoder_7seg_1_f_e,
			f_f =>  decoder_7seg_1_f_f,
			f_g =>  decoder_7seg_1_f_g
		);

	-- gnd_2
	gnd_2_O <= '0';

	-- hodiny_0
	inst_hodiny_0 : hodiny
		port map (
			clk_100MHz =>  clk_100MHz_0_1,
			reset      =>  gnd_2_O   ,
			mux_sel    =>  hodiny_0_mux_sel
		);

	-- inv_0
	inv_0_O <= not(hodiny_0_mux_sel);

	-- mux_0
	mux_0_O_0 <= decoder_7seg_1_f_a when (hodiny_0_mux_sel = '1') else decoder_7seg_0_f_a;
	mux_0_O_1 <= decoder_7seg_1_f_b when (hodiny_0_mux_sel = '1') else decoder_7seg_0_f_b;
	mux_0_O_2 <= decoder_7seg_1_f_c when (hodiny_0_mux_sel = '1') else decoder_7seg_0_f_c;
	mux_0_O_3 <= decoder_7seg_1_f_d when (hodiny_0_mux_sel = '1') else decoder_7seg_0_f_d;
	mux_0_O_4 <= decoder_7seg_1_f_e when (hodiny_0_mux_sel = '1') else decoder_7seg_0_f_e;
	mux_0_O_5 <= decoder_7seg_1_f_f when (hodiny_0_mux_sel = '1') else decoder_7seg_0_f_f;
	mux_0_O_6 <= decoder_7seg_1_f_g when (hodiny_0_mux_sel = '1') else decoder_7seg_0_f_g;

	-- vcc_1
	vcc_1_O <= '1';

	-- vcc_2
	vcc_2_O <= '1';

end Behavioral;
