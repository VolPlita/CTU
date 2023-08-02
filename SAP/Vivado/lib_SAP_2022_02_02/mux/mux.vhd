----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 15.01.2018 18:38:11
-- Design Name: 
-- Module Name: mux - Behavioral
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

entity mux is
    Generic (WIDTH: integer := 4;
            I0_BUS, I1_BUS, I2_BUS, I3_BUS: boolean := false 
            );
    Port (I0: in STD_LOGIC_VECTOR(WIDTH-1 downto 0);
          I0_0, I0_1, I0_2, I0_3, I0_4, I0_5, I0_6, I0_7: in STD_LOGIC;
          I1: in STD_LOGIC_VECTOR(WIDTH-1 downto 0);
          I1_0, I1_1, I1_2, I1_3, I1_4, I1_5, I1_6, I1_7: in STD_LOGIC;
          I2: in STD_LOGIC_VECTOR(WIDTH-1 downto 0);
          I2_0, I2_1, I2_2, I2_3, I2_4, I2_5, I2_6, I2_7: in STD_LOGIC;
          I3: in STD_LOGIC_VECTOR(WIDTH-1 downto 0);
          I3_0, I3_1, I3_2, I3_3, I3_4, I3_5, I3_6, I3_7: in STD_LOGIC;
          Sel0, Sel1: in STD_LOGIC;
          O_0, O_1, O_2, O_3, O_4, O_5, O_6, O_7: out STD_LOGIC;
          O: out STD_LOGIC_VECTOR(WIDTH-1 downto 0)
         );
end mux;

architecture Behavioral of mux is

    signal i0_internal, i1_internal, i2_internal, i3_internal: STD_LOGIC_VECTOR(7 downto 0);
    signal sel_internal: STD_LOGIC_VECTOR(1 downto 0);
    signal o_internal: STD_LOGIC_VECTOR(7 downto 0);

begin

    i0signal: if (not I0_BUS) generate
        i0_internal <= I0_7 & I0_6 & I0_5 & I0_4 & I0_3 & I0_2 & I0_1 & I0_0;
    end generate;
    i0bus: if (I0_BUS) generate
        i0_internal <= std_logic_vector(resize(unsigned(I0), 8));
--        i0_internal(WIDTH-1 downto 0) <= I0;
--        i0_internal(7 downto WIDTH) <= (others => '0');
    end generate;
    
        
    i1signal: if (not I1_BUS) generate
        i1_internal <= I1_7 & I1_6 & I1_5 & I1_4 & I1_3 & I1_2 & I1_1 & I1_0;
    end generate;
    i1bus: if (I1_BUS) generate
        i1_internal <= std_logic_vector(resize(unsigned(I1), 8));
--        i1_internal(WIDTH-1 downto 0) <= I1;
--        i1_internal(7 downto WIDTH) <= (others => '0');
    end generate;
    
        
    i2signal: if (not I2_BUS) generate
        i2_internal <= I2_7 & I2_6 & I2_5 & I2_4 & I2_3 & I2_2 & I2_1 & I2_0;
    end generate;
    i2bus: if (I2_BUS) generate
        i2_internal <= std_logic_vector(resize(unsigned(I2), 8));
--        i2_internal(WIDTH-1 downto 0) <= I2;
--        i2_internal(7 downto WIDTH) <= (others => '0');
    end generate;
    
        
    i3signal: if (not I3_BUS) generate
        i3_internal <= I3_7 & I3_6 & I3_5 & I3_4 & I3_3 & I3_2 & I3_1 & I3_0;
    end generate;
    i3bus: if (I3_BUS) generate
        i3_internal <= std_logic_vector(resize(unsigned(I3), 8));
--        i3_internal(WIDTH-1 downto 0) <= I3;
--        i3_internal(7 downto WIDTH) <= (others => '0');
    end generate;


    sel_internal <= Sel1 & Sel0;
    
    with sel_internal select
       o_internal <= i3_internal when "11",
                 i2_internal when "10",
                 i1_internal when "01",
                 i0_internal when others;
                 
    
    O <= o_internal(WIDTH-1 downto 0);

    O_0 <= o_internal(0);
    O_1 <= o_internal(1);
    O_2 <= o_internal(2);
    O_3 <= o_internal(3);
    O_4 <= o_internal(4);
    O_5 <= o_internal(5);
    O_6 <= o_internal(6);
    O_7 <= o_internal(7);   


end Behavioral;
