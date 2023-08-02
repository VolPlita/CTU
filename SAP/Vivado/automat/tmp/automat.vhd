library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity automat_vhdl is
   port (
      clk   :  in  std_logic;
      INC   :  in  std_logic;
      reset :  in  std_logic;
      y0    :  out std_logic;
      y1    :  out std_logic;
      y2    :  out std_logic
   );
end entity automat_vhdl;

architecture Behavioral of automat_vhdl is
	component next_state_logic
		port (
			DIR :  in  std_logic;
			INC :  in  std_logic;
			q0  :  in  std_logic;
			q1  :  in  std_logic;
			q2  :  in  std_logic;
			d0  :  out std_logic;
			d1  :  out std_logic;
			d2  :  out std_logic
		);
	end component;

	component output_logic
		port (
			DIR :  in  std_logic;
			INC :  in  std_logic;
			q0  :  in  std_logic;
			q1  :  in  std_logic;
			q2  :  in  std_logic;
			y0  :  out std_logic;
			y1  :  out std_logic;
			y2  :  out std_logic
		);
	end component;



      signal clk_1                 :    std_logic;         
      signal dff_0_q               :    std_logic;         
      signal dff_1_q               :    std_logic;         
      signal dff_2_q               :    std_logic;         
      signal gnd_0_O               :    std_logic;         
      signal INC_1                 :    std_logic;         
      signal next_state_logic_0_d0 :    std_logic;         
      signal next_state_logic_0_d1 :    std_logic;         
      signal next_state_logic_0_d2 :    std_logic;         
      signal output_logic_0_y0     :    std_logic;         
      signal output_logic_0_y1     :    std_logic;         
      signal output_logic_0_y2     :    std_logic;         
      signal reset_1               :    std_logic;         
begin
	clk_1 <= clk;
	INC_1 <= INC;
	reset_1 <= reset;
	y0 <= output_logic_0_y0;
	y1 <= output_logic_0_y1;
	y2 <= output_logic_0_y2;


	-- dff_0
	process (clk_1)
	begin
		if rising_edge(clk_1) then
			if reset_1 = '1' then
				dff_0_q <= '0';
			else
				dff_0_q <= next_state_logic_0_d0;
			end if;
		end if;
	end process;

	-- dff_1
	process (clk_1)
	begin
		if rising_edge(clk_1) then
			if reset_1 = '1' then
				dff_1_q <= '0';
			else
				dff_1_q <= next_state_logic_0_d1;
			end if;
		end if;
	end process;

	-- dff_2
	process (clk_1)
	begin
		if rising_edge(clk_1) then
			if reset_1 = '1' then
				dff_2_q <= '0';
			else
				dff_2_q <= next_state_logic_0_d2;
			end if;
		end if;
	end process;

	-- gnd_0
	gnd_0_O <= '0';

	-- next_state_logic_0
	inst_next_state_logic_0 : next_state_logic
		port map (
			DIR =>  gnd_0_O,
			INC =>  INC_1,
			q0  =>  dff_0_q,
			q1  =>  dff_1_q,
			q2  =>  dff_2_q,
			d0  =>  next_state_logic_0_d0,
			d1  =>  next_state_logic_0_d1,
			d2  =>  next_state_logic_0_d2
		);

	-- output_logic_0
	inst_output_logic_0 : output_logic
		port map (
			DIR =>  gnd_0_O,
			INC =>  gnd_0_O,
			q0  =>  dff_0_q,
			q1  =>  dff_1_q,
			q2  =>  dff_2_q,
			y0  =>  output_logic_0_y0,
			y1  =>  output_logic_0_y1,
			y2  =>  output_logic_0_y2
		);

end Behavioral;
