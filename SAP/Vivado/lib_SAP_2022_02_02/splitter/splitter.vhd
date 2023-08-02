----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 15.01.2018 18:38:11
-- Design Name: 
-- Module Name: splitter - Behavioral
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
use IEEE.numeric_std.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity splitter is
    Generic (WIDTH: integer := 4
            );
    Port (I: in STD_LOGIC_VECTOR(WIDTH-1 downto 0);
          O0, O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13, O14, O15, O16, O17, O18, O19, O20, O21, O22, O23, O24, O25, O26, O27, O28, O29, O30, O31 : out STD_LOGIC
         );
end splitter;

architecture Behavioral of splitter is

    signal temp: std_logic_vector(31 downto 0);
    
begin

    temp <= std_logic_vector(resize(unsigned(I), 32));

    O0 <= temp(0);  
    O1 <= temp(1);  
    O2 <= temp(2);  
    O3 <= temp(3);  
    O4 <= temp(4);  
    O5 <= temp(5);  
    O6 <= temp(6);  
    O7 <= temp(7);  
    O8 <= temp(8);  
    O9 <= temp(9);  
    O10 <= temp(10);  
    O11 <= temp(11);  
    O12 <= temp(12);  
    O13 <= temp(13);  
    O14 <= temp(14);  
    O15 <= temp(15);  
    O16 <= temp(16);  
    O17 <= temp(17);  
    O18 <= temp(18);  
    O19 <= temp(19);  
    O20 <= temp(20);  
    O21 <= temp(21);  
    O22 <= temp(22);  
    O23 <= temp(23);  
    O24 <= temp(24);  
    O25 <= temp(25);  
    O26 <= temp(26);  
    O27 <= temp(27);  
    O28 <= temp(28);  
    O29 <= temp(29);  
    O30 <= temp(30);  
    O31 <= temp(31);  


end Behavioral;
