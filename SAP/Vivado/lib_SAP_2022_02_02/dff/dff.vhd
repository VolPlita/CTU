----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11.01.2018 19:21:56
-- Design Name: 
-- Module Name: dff - Behavioral
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

entity dff is
port(
    d: in std_logic;
    clk: in std_logic;
    ce: in std_logic;
    reset: in std_logic;
    q: out std_logic
);
--  Port ( );
end dff;

architecture Behavioral of dff is
begin

process (clk)
begin
   if clk'event and clk='1' then
      if reset='1' then
         q <= '0';
      elsif ce='1' then
         q <= d;
      end if;
   end if;
end process;


end Behavioral;
