library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity automatd_vhdl is
   port (
      clk_0   :  in  std_logic;
      clk_1   :  in  std_logic;
      reset_0 :  in  std_logic;
      tl_in_0 :  in  std_logic;
      AN0     :  out std_logic;
      AN1     :  out std_logic;
      AN2     :  out std_logic;
      AN3     :  out std_logic;
      f_a_0   :  out std_logic;
      f_b_0   :  out std_logic;
      f_c_0   :  out std_logic;
      f_d_0   :  out std_logic;
      f_e_0   :  out std_logic;
      f_f_0   :  out std_logic;
      f_g_0   :  out std_logic
   );
end entity automatd_vhdl;

architecture Behavioral of automatd_vhdl is
	component automat_vhdl
		port (
			clk   :  in  std_logic;
			INC   :  in  std_logic;
			reset :  in  std_logic;
			y0    :  out std_logic;
			y1    :  out std_logic;
			y2    :  out std_logic
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

	component debounce
		generic (
			width: integer := 22
		);
		port (
			clk    : in  std_logic;
			tl_in  : in  std_logic;
			tl_out : out std_logic
		);
	end component;


      signal automat_vhdl_0_y0  :    std_logic;      
      signal automat_vhdl_0_y1  :    std_logic;      
      signal automat_vhdl_0_y2  :    std_logic;      
      signal clk_0_1            :    std_logic;      
      signal clk_1_1            :    std_logic;      
      signal debounce_0_tl_out  :    std_logic;      
      signal decoder_7seg_0_f_a :    std_logic;      
      signal decoder_7seg_0_f_b :    std_logic;      
      signal decoder_7seg_0_f_c :    std_logic;      
      signal decoder_7seg_0_f_d :    std_logic;      
      signal decoder_7seg_0_f_e :    std_logic;      
      signal decoder_7seg_0_f_f :    std_logic;      
      signal decoder_7seg_0_f_g :    std_logic;      
      signal gnd_0_O            :    std_logic;      
      signal reset_0_1          :    std_logic;      
      signal tl_in_0_1          :    std_logic;      
      signal vcc_0_O            :    std_logic;      
begin
	clk_0_1 <= clk_0;
	clk_1_1 <= clk_1;
	reset_0_1 <= reset_0;
	tl_in_0_1 <= tl_in_0;
	AN0 <= gnd_0_O;
	AN1 <= vcc_0_O;
	AN2 <= vcc_0_O;
	AN3 <= vcc_0_O;
	f_a_0 <= decoder_7seg_0_f_a;
	f_b_0 <= decoder_7seg_0_f_b;
	f_c_0 <= decoder_7seg_0_f_c;
	f_d_0 <= decoder_7seg_0_f_d;
	f_e_0 <= decoder_7seg_0_f_e;
	f_f_0 <= decoder_7seg_0_f_f;
	f_g_0 <= decoder_7seg_0_f_g;


	-- automat_vhdl_0
	inst_automat_vhdl_0 : automat_vhdl
		port map (
			clk   =>  clk_0_1,
			INC   =>  debounce_0_tl_out,
			reset =>  reset_0_1,
			y0    =>  automat_vhdl_0_y0,
			y1    =>  automat_vhdl_0_y1,
			y2    =>  automat_vhdl_0_y2
		);

	-- debounce_0
	debounce_0 : debounce
		generic map (
			width => 22
		)
		port map (
			clk	=> clk_1_1,
			tl_in	=> tl_in_0_1,
			tl_out	=> debounce_0_tl_out
		);

	-- decoder_7seg_0
	inst_decoder_7seg_0 : decoder_7seg
		port map (
			a   =>  automat_vhdl_0_y0,
			b   =>  automat_vhdl_0_y1,
			c   =>  automat_vhdl_0_y2,
			d   =>  gnd_0_O,
			f_a =>  decoder_7seg_0_f_a,
			f_b =>  decoder_7seg_0_f_b,
			f_c =>  decoder_7seg_0_f_c,
			f_d =>  decoder_7seg_0_f_d,
			f_e =>  decoder_7seg_0_f_e,
			f_f =>  decoder_7seg_0_f_f,
			f_g =>  decoder_7seg_0_f_g
		);

	-- gnd_0
	gnd_0_O <= '0';

	-- vcc_0
	vcc_0_O <= '1';

end Behavioral;


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

entity debounce is
    generic (
		width: integer := 22 -- 100 MHz clock -> 4 - simulation (90 ns); 22 - synthesis (ca. 20 ms)
	);
    port (
		clk : in STD_LOGIC;
        tl_in : in STD_LOGIC;
        tl_out : out STD_LOGIC
	);
end entity debounce;

architecture Behavioral of debounce is

signal cnt       : std_logic_vector(width-1 downto 0) := (others => '0');
signal reset, ce : std_logic := '0';
signal tl_prev   : std_logic := '0';


begin

    counter: process(clk) begin
        if rising_edge(clk) then 
            if (reset = '1') then
                cnt <= (others => '0');
            elsif (ce = '1') then
                cnt <= cnt + 1;
            end if;
        end if;
    end process;

    process(clk) begin
        if rising_edge(clk) then
            if (tl_prev /= tl_in) then
                reset <= '1';
            else 
                reset <= '0';
            end if;
        end if;
    end process;
                
    process(clk) begin
        if rising_edge(clk) then
            if (reset = '1') then
                tl_prev <= tl_in;
            end if;
        end if;
    end process;
                
    process(clk) begin
        if rising_edge(clk) then
            if ((cnt(width-1) = '1') and (cnt(0) = '0')) then
                tl_out <= tl_prev;
            end if;
        end if;
    end process;
                

    ce <= '0' when ((cnt(width-1) = '1') and (cnt(0) = '1'))
                else '1';

end Behavioral;

