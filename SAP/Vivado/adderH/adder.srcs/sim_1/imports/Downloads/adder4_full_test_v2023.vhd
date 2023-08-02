-- knihovna typù pro práci
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
  
-- prázdná entita zapouzdøující testovací sadu
ENTITY adder4_full_test IS
END adder4_full_test;
 
-- popis chování testovací sady
-- hlavièka ve formatu: ARCHITECTURE <libovolny_nazev> OF <nazev_entity> IS
ARCHITECTURE behavior OF adder4_full_test IS 
 
    -- definice vstupù a výstupù testované komponenty
    -- vstupy:  CIN, A0..A3, B0..B3 
    -- výstupy: S0..S3, COUT
    -- POZOR!
    -- NÁZEV KOMPONENTY (COMPONENT <nazev>) MUSÍ BÝT STEJNÝ JAKO NÁZEV TESTOVANÉHO SCHÉMATU
    -- V TOMTO PØÍPADÌ: COMPONENT scit4
    -- VSTUPY A VÝSTUPY KOMPONENTY (COMPONENT scit4) MUSÍ BÝT STEJNÉ JAKO POPISY VSTUPÙ A VÝSTUPÙ VE SCHÉMATU

    COMPONENT adder_4_vdhl
    PORT(
         CIN    : IN  std_logic;
         A0    : IN  std_logic;
         B0    : IN  std_logic;
         A1    : IN  std_logic;
         B1    : IN  std_logic;
         A2    : IN  std_logic;
         B2    : IN  std_logic;
         A3    : IN  std_logic;
         B3    : IN  std_logic;
         S0    : OUT  std_logic;
         S1    : OUT  std_logic;
         S2    : OUT  std_logic;
         S3    : OUT  std_logic;
         COUT    : OUT  std_logic
        );
    END COMPONENT;
    
   signal A3,A2,A1,A0    : std_logic;
   signal B3,B2,B1,B0    : std_logic; 
   signal S3,S2,S1,S0    : std_logic;
   signal COUT : std_logic;
	signal a,b : std_logic_vector(3 downto 0);
	signal CIN    : std_logic;
	signal s : std_logic_vector(4 downto 0);
	
BEGIN
	s <= cout & s3 & s2 & s1 & s0;
   -- mapování vstupù a výstupù testované komponenty na signály ovládané testovací sadou
   uut: adder_4_vdhl PORT MAP (
      CIN => CIN,
      A0 => A0,
      B0 => B0,
      A1 => A1,
      B1 => B1,
      A2 => A2,
      B2 => B2,
      A3 => A3,
      B3 => B3,
      S0 => S0,
      S1 => S1,
      S2 => S2,
      S3 => S3,
      COUT => COUT
   );
 
   -- simulaèní proces realizující chování testovací sady
   stim_proc: process
		variable aa,bb : std_logic_vector(3 downto 0);
   begin      
      -- poèáteèní nastavení vstupù
      CIN    <= '0';
      A0    <= '0';
      B0    <= '0'; 
      A1    <= '0';
      B1    <= '0';
      A2    <= '0';
      B2    <= '0';   
      A3    <= '0';
      B3    <= '0';  
		a <= (others => '0');
		b <= (others => '0');
      wait for 10 ns;
		
		aa := "0000";
		bb := "0000";
   
		for i in 0 to 15 loop
			A0    <= aa(0);
			A1    <= aa(1);
			A2    <= aa(2);  
			A3    <= aa(3);
			a <= aa;
			
			wait for 1.5 ns;
			
			for j in 0 to 15 loop
				b0    <= bb(0);
				b1    <= bb(1);
				b2    <= bb(2);  
				b3    <= bb(3);
				b <= bb;
				wait for 0 ns;
				
				
				wait for 1.5 ns;
				--assert (conv_integer(a) + conv_integer(b) = conv_integer(s)) report "Chyba" severity error;
				assert (conv_integer(aa) + conv_integer(bb) + conv_integer(cin)  = conv_integer(s)) report "Chyba pri scitani "&integer'image(conv_integer(aa))&" + "&integer'image(conv_integer(bb))& " + "&integer'image(conv_integer(cin))&". Vyslo "&integer'image(conv_integer(s)) severity error;
				bb := bb + 1;
			end loop;
			
			aa := aa + 1;
		end loop;
		
		cin <= '1';
		
		      wait for 10 ns;
		
		aa := "0000";
		bb := "0000";
   
		for i in 0 to 15 loop
			A0    <= aa(0);
			A1    <= aa(1);
			A2    <= aa(2);  
			A3    <= aa(3);
			a <= aa;
			
			wait for 1.5 ns;
			
			for j in 0 to 15 loop
				b0    <= bb(0);
				b1    <= bb(1);
				b2    <= bb(2);  
				b3    <= bb(3);
				b <= bb;
				wait for 0 ns;
				
				
				wait for 1.5 ns;
				--assert (conv_integer(a) + conv_integer(b) + 1 = conv_integer(s)) report "Chyba" severity error;
				assert (conv_integer(aa) + conv_integer(bb) + conv_integer(cin) = conv_integer(s)) report "Chyba pri scitani "&integer'image(conv_integer(aa))&" + "&integer'image(conv_integer(bb))& " + "&integer'image(conv_integer(cin))&". Vyslo "&integer'image(conv_integer(s)) severity error;
				bb := bb + 1;
			end loop;
			
			aa := aa + 1;
		end loop;
      
		assert false report "Pokud se neobjevila zadna jina hlaska, je vse OK." severity note;
      wait;   -- nekoneèná èekací smyèka
   end process;

END;
