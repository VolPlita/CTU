library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity automat_vhdl is
   port (
      b1    :  in  std_logic;
      b2    :  in  std_logic;
      CLK   :  in  std_logic;
      RESET :  in  std_logic;
      y0    :  out std_logic
   );
end entity automat_vhdl;

architecture Behavioral of automat_vhdl is
	component next_state_logic
		port (
			a1 :  in  std_logic;
			a2 :  in  std_logic;
			b1 :  in  std_logic;
			b2 :  in  std_logic;
			n1 :  out std_logic;
			n2 :  out std_logic
		);
	end component;

	component output_logic
		port (
			a1 :  in  std_logic;
			a2 :  in  std_logic;
			b1 :  in  std_logic;
			b2 :  in  std_logic;
			y  :  out std_logic
		);
	end component;



      signal b1_1                  :    std_logic;         
      signal b2_1                  :    std_logic;         
      signal CLK_1                 :    std_logic;         
      signal dff_0_q               :    std_logic;         
      signal dff_1_q               :    std_logic;         
      signal next_state_logic_0_n1 :    std_logic;         
      signal next_state_logic_0_n2 :    std_logic;         
      signal output_logic_0_y      :    std_logic;         
      signal RESET_1               :    std_logic;         
begin
	b1_1 <= b1;
	b2_1 <= b2;
	CLK_1 <= CLK;
	RESET_1 <= RESET;
	y0 <= output_logic_0_y;


	-- dff_0
	process (CLK_1)
	begin
		if rising_edge(CLK_1) then
			if RESET_1 = '1' then
				dff_0_q <= '0';
			else
				dff_0_q <= next_state_logic_0_n1;
			end if;
		end if;
	end process;

	-- dff_1
	process (CLK_1)
	begin
		if rising_edge(CLK_1) then
			if RESET_1 = '1' then
				dff_1_q <= '0';
			else
				dff_1_q <= next_state_logic_0_n2;
			end if;
		end if;
	end process;

	-- next_state_logic_0
	inst_next_state_logic_0 : next_state_logic
		port map (
			a1 =>  dff_0_q,
			a2 =>  dff_1_q,
			b1 =>  b1_1,
			b2 =>  b2_1,
			n1 =>  next_state_logic_0_n1,
			n2 =>  next_state_logic_0_n2
		);

	-- output_logic_0
	inst_output_logic_0 : output_logic
		port map (
			a1 =>  dff_0_q,
			a2 =>  dff_1_q,
			b1 =>  b1_1,
			b2 =>  b2_1,
			y  =>  output_logic_0_y
		);

end Behavioral;
