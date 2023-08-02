library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity decoder_vhdl is
   port (
      A0  :  in  std_logic;
      B0  :  in  std_logic;
      C0  :  in  std_logic;
      D0  :  in  std_logic;
      f_a :  out std_logic;
      f_b :  out std_logic;
      f_c :  out std_logic;
      f_d :  out std_logic;
      f_e :  out std_logic;
      f_f :  out std_logic;
      f_g :  out std_logic
   );
end entity decoder_vhdl;

architecture Behavioral of decoder_vhdl is
	component f_a_vhdl
		port (
			A0  :  in  std_logic;
			B0  :  in  std_logic;
			C0  :  in  std_logic;
			D0  :  in  std_logic;
			f_a :  out std_logic
		);
	end component;

	component f_b_vhdl
		port (
			A0  :  in  std_logic;
			B0  :  in  std_logic;
			C0  :  in  std_logic;
			D0  :  in  std_logic;
			f_b :  out std_logic
		);
	end component;

	component f_c_vhdl
		port (
			A0  :  in  std_logic;
			B0  :  in  std_logic;
			C0  :  in  std_logic;
			D0  :  in  std_logic;
			f_c :  out std_logic
		);
	end component;

	component f_d_vhdl
		port (
			A0  :  in  std_logic;
			B0  :  in  std_logic;
			C0  :  in  std_logic;
			D0  :  in  std_logic;
			f_d :  out std_logic
		);
	end component;

	component f_e_vhdl
		port (
			A0  :  in  std_logic;
			B0  :  in  std_logic;
			C0  :  in  std_logic;
			D0  :  in  std_logic;
			f_e :  out std_logic
		);
	end component;

	component f_f_vhdl
		port (
			A0  :  in  std_logic;
			B0  :  in  std_logic;
			C0  :  in  std_logic;
			D0  :  in  std_logic;
			f_f :  out std_logic
		);
	end component;

	component f_g_vhdl
		port (
			A0  :  in  std_logic;
			B0  :  in  std_logic;
			C0  :  in  std_logic;
			D0  :  in  std_logic;
			f_g :  out std_logic
		);
	end component;



      signal A0_0_1         :    std_logic;  
      signal B0_0_1         :    std_logic;  
      signal C0_0_1         :    std_logic;  
      signal D0_0_1         :    std_logic;  
      signal f_a_vhdl_0_f_a :    std_logic;  
      signal f_b_vhdl_0_f_b :    std_logic;  
      signal f_c_vhdl_0_f_c :    std_logic;  
      signal f_d_vhdl_0_f_d :    std_logic;  
      signal f_e_vhdl_0_f_e :    std_logic;  
      signal f_f_vhdl_0_f_f :    std_logic;  
      signal f_g_vhdl_0_f_g :    std_logic;  
begin
	A0_0_1 <= A0;
	B0_0_1 <= B0;
	C0_0_1 <= C0;
	D0_0_1 <= D0;
	f_a <= f_a_vhdl_0_f_a;
	f_b <= f_b_vhdl_0_f_b;
	f_c <= f_c_vhdl_0_f_c;
	f_d <= f_d_vhdl_0_f_d;
	f_e <= f_e_vhdl_0_f_e;
	f_f <= f_f_vhdl_0_f_f;
	f_g <= f_g_vhdl_0_f_g;


	-- f_a_vhdl_0
	inst_f_a_vhdl_0 : f_a_vhdl
		port map (
			A0  =>  A0_0_1,
			B0  =>  B0_0_1,
			C0  =>  C0_0_1,
			D0  =>  D0_0_1,
			f_a =>  f_a_vhdl_0_f_a
		);

	-- f_b_vhdl_0
	inst_f_b_vhdl_0 : f_b_vhdl
		port map (
			A0  =>  A0_0_1,
			B0  =>  B0_0_1,
			C0  =>  C0_0_1,
			D0  =>  D0_0_1,
			f_b =>  f_b_vhdl_0_f_b
		);

	-- f_c_vhdl_0
	inst_f_c_vhdl_0 : f_c_vhdl
		port map (
			A0  =>  A0_0_1,
			B0  =>  B0_0_1,
			C0  =>  C0_0_1,
			D0  =>  D0_0_1,
			f_c =>  f_c_vhdl_0_f_c
		);

	-- f_d_vhdl_0
	inst_f_d_vhdl_0 : f_d_vhdl
		port map (
			A0  =>  A0_0_1,
			B0  =>  B0_0_1,
			C0  =>  C0_0_1,
			D0  =>  D0_0_1,
			f_d =>  f_d_vhdl_0_f_d
		);

	-- f_e_vhdl_0
	inst_f_e_vhdl_0 : f_e_vhdl
		port map (
			A0  =>  A0_0_1,
			B0  =>  B0_0_1,
			C0  =>  C0_0_1,
			D0  =>  D0_0_1,
			f_e =>  f_e_vhdl_0_f_e
		);

	-- f_f_vhdl_0
	inst_f_f_vhdl_0 : f_f_vhdl
		port map (
			A0  =>  A0_0_1,
			B0  =>  B0_0_1,
			C0  =>  C0_0_1,
			D0  =>  D0_0_1,
			f_f =>  f_f_vhdl_0_f_f
		);

	-- f_g_vhdl_0
	inst_f_g_vhdl_0 : f_g_vhdl
		port map (
			A0  =>  A0_0_1,
			B0  =>  B0_0_1,
			C0  =>  C0_0_1,
			D0  =>  D0_0_1,
			f_g =>  f_g_vhdl_0_f_g
		);

end Behavioral;
