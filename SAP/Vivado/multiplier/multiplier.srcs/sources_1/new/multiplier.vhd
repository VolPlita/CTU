----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/05/2023 08:06:00 PM
-- Design Name: 
-- Module Name: multiplier - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity multiplier is
    Port ( d : in STD_LOGIC;
           c : in STD_LOGIC;
           b : in STD_LOGIC;
           a : in STD_LOGIC;
           f_a : out STD_LOGIC;
           f_b : out STD_LOGIC;
           f_c : out STD_LOGIC;
           f_d : out STD_LOGIC;
           f_e : out STD_LOGIC;
           f_f : out STD_LOGIC;
           f_g : out STD_LOGIC);
end multiplier;

architecture Behavioral of multiplier is

begin
    
    f_a <= ( (a and (not b) and c and (not d)) or ((not a) and b and (not c) and d) );
    f_b <= ( ((not a) and b and c and d) or (a and b and (not c) and d) );
    f_c <= ( ((not a) and b and c and (not d)) or (a and (not b) and (not c) and d) );
	f_d <= ( (a and (not b) and c and (not d)) or ((not a) and b and (not c) and d) );
	f_e <= ( (a and c) or ((not a) and b and (not c) and d) );
	f_f <= (not( ((not c) and (not d)) or ((not a) and (not b)) or (b and d) ));
    f_g <= ( ((not c) and (not d)) or ((not a) and (not b)) or ((not b) and (not d)) );
  
end Behavioral;
