library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


-- Popis vstup� a v�stup� 7-segmentov�ho dekod�ru
--    vstupy: d, c, b, a
--    v�stupy: f_a, f_b, f_c, f_d, f_e, f_f, f_g
-- v�stupy f_a ... f_g ovl�daj� jednotliv� segmenty displeje
entity decoder_7seg is
   Port ( 
      d : in  STD_LOGIC;
      c : in  STD_LOGIC;
      b : in  STD_LOGIC;
      a : in  STD_LOGIC;
      f_a : out  STD_LOGIC;
      f_b : out  STD_LOGIC;
      f_c : out  STD_LOGIC;
      f_d : out  STD_LOGIC;
      f_e : out  STD_LOGIC;
      f_f : out  STD_LOGIC;
      f_g : out  STD_LOGIC
   );
end decoder_7seg;                                       
-- popis chov�n� 7-segmentov�ho dekod�ru
architecture decoder_7seg_arch of decoder_7seg is       
   -- vnit�n� sign�l, kter� se pou��v� k rozli�en� jednotliv�ch vstup�
   signal   input :  std_logic_vector (3 downto 0);     
   
begin

   -- zpracov�n� vstup� pro jednodu��� pr�ci s tabulkou 
   input(3) <= d;
   input(2) <= c;
   input(1) <= b;
   input(0) <= a;
	
-- Zn�zorn�n� 7-segmentov�ho displeje
-- (v�ce se m��ete dozv�d�t v referen�n�m manu�lu p��pravku Basys-2)       
--       f_A  
--       --- 
--  f_F  |   |  f_B     
--       ---    <- f_G  
--  f_E  |   |  f_C                                    
--       ---                              
--       f_D
--
--  Ka�d� segment je zapnut� (rozsv�cen�) hodnotou log. 0
--
--  ��sla 0..9 zobrazujte podle obecn� zn�m� konvence,
--  pro ostatn� hodnoty (A..F) pou�ijte n�sleduj�c� zobrazen�:
--  A:      B:      C:      D:      E:      F:
--    ---             ---             ---     ---
--   |   |   |       |           |   |       |
--    ---     ---             ---     ---     ---
--   |   |   |   |   |       |   |   |       |
--            ---     ---     ---     ---

   -------------------------------------------------------------------
   -------------------------------------------------------------------
   -- Zde dopl�te 3 algebraick� v�razy pro segmenty CA, CB, CC      --
   -------------------------------------------------------------------
   -------------------------------------------------------------------
	
   f_a <= (not( ((not a) and (not c)) or (b and (not d)) or (b and c) or (a and c and (not d)) or ((not b) and (not c) and d) or ((not a) and c and d) ));
   f_b <= (not( ((not a) and (not c)) or ((not c) and (not d)) or ((not a) and (not b) and (not d)) or (a and b and (not d)) or (a and (not b) and d) ));
   f_c <= (not( ((not b) and (not d)) or (a and (not d)) or (a and (not b)) or (c and (not d)) or ((not c) and d) ));
	
   -------------------------------------------------------------------
   -------------------------------------------------------------------
   -- Zde dopl�te v�razy tabulkou pro zbyl� segmenty CD, CE, CF, CG --
   -------------------------------------------------------------------
   -------------------------------------------------------------------
	
   -- ------------------------------------------------------------------------------
   -- |  fun�kn� hodnota f_d(d, c, b, a) |  stavov� index s  |  d  |  c  |  b  |  a  | 
   -- ------------------------------------------------------------------------------
   with input select
   f_d <= '0'   when	"0000",         -- |           0       |  0  |  0  |  0  |  0  |       
          '1'   when	"0001",         -- |           1       |  0  |  0  |  0  |  1  |
          '0'   when	"0010",         -- |           2       |  0  |  0  |  1  |  0  |
          '0'   when	"0011",         -- |           3       |  0  |  0  |  1  |  1  |
          '1'   when	"0100",         -- |           4       |  0  |  1  |  0  |  0  |
          '0'   when	"0101",         -- |           5       |  0  |  1  |  0  |  1  |
          '0'   when	"0110",         -- |           6       |  0  |  1  |  1  |  0  |
          '1'   when	"0111",         -- |           7       |  0  |  1  |  1  |  1  |
          '0'   when	"1000",         -- |           8       |  1  |  0  |  0  |  0  |
          '0'   when	"1001",         -- |           9       |  1  |  0  |  0  |  1  |
          '1'   when	"1010",         -- |          10       |  1  |  0  |  1  |  0  |
          '0'   when	"1011",         -- |          11       |  1  |  0  |  1  |  1  |
          '0'   when	"1100",         -- |          12       |  1  |  1  |  0  |  0  |
          '0'   when	"1101",         -- |          13       |  1  |  1  |  0  |  1  |
          '0'   when	"1110",         -- |          14       |  1  |  1  |  1  |  0  |
          '1'   when	"1111",         -- |          15       |  1  |  1  |  1  |  1  |
          '0'   when others;
   
   -- ------------------------------------------------------------------------------;
   -- ------------------------------------------------------------------------------
   -- |  fun�kn� hodnota f_e(d, c, b, a) |  stavov� index s  |  d  |  c  |  b  |  a  | 
   -- ------------------------------------------------------------------------------
   with input select
   f_e <= '0'   when	"0000",         -- |           0       |  0  |  0  |  0  |  0  |  
          '1'   when	"0001",         -- |           1       |  0  |  0  |  0  |  1  |
          '0'   when	"0010",         -- |           2       |  0  |  0  |  1  |  0  |
          '1'   when	"0011",         -- |           3       |  0  |  0  |  1  |  1  |
          '1'   when	"0100",         -- |           4       |  0  |  1  |  0  |  0  |
          '1'   when	"0101",         -- |           5       |  0  |  1  |  0  |  1  |
          '0'   when	"0110",         -- |           6       |  0  |  1  |  1  |  0  |
          '1'   when	"0111",         -- |           7       |  0  |  1  |  1  |  1  |
          '0'   when	"1000",         -- |           8       |  1  |  0  |  0  |  0  |
          '1'   when	"1001",         -- |           9       |  1  |  0  |  0  |  1  |
          '0'   when	"1010",         -- |          10       |  1  |  0  |  1  |  0  |
          '0'   when	"1011",         -- |          11       |  1  |  0  |  1  |  1  |
          '0'   when	"1100",         -- |          12       |  1  |  1  |  0  |  0  |
          '0'   when	"1101",         -- |          13       |  1  |  1  |  0  |  1  |
          '0'   when	"1110",         -- |          14       |  1  |  1  |  1  |  0  |
          '0'   when	"1111",         -- |          15       |  1  |  1  |  1  |  1  |
          '0'   when others;
   
   -- ------------------------------------------------------------------------------;
   -- ------------------------------------------------------------------------------
   -- |  fun�kn� hodnota f_f(d, c, b, a) |  stavov� index s  |  d  |  c  |  b  |  a  | 
   -- ------------------------------------------------------------------------------
   with input select
   f_f <= '0'   when	"0000",         -- |           0       |  0  |  0  |  0  |  0  |            
          '1'   when	"0001",         -- |           1       |  0  |  0  |  0  |  1  |
          '1'   when	"0010",         -- |           2       |  0  |  0  |  1  |  0  |
          '1'   when	"0011",         -- |           3       |  0  |  0  |  1  |  1  |
          '0'   when	"0100",         -- |           4       |  0  |  1  |  0  |  0  |
          '0'   when	"0101",         -- |           5       |  0  |  1  |  0  |  1  |
          '0'   when	"0110",         -- |           6       |  0  |  1  |  1  |  0  |
          '1'   when	"0111",         -- |           7       |  0  |  1  |  1  |  1  |
          '0'   when	"1000",         -- |           8       |  1  |  0  |  0  |  0  |
          '0'   when	"1001",         -- |           9       |  1  |  0  |  0  |  1  |
          '0'   when	"1010",         -- |          10       |  1  |  0  |  1  |  0  |
          '0'   when	"1011",         -- |          11       |  1  |  0  |  1  |  1  |
          '0'   when	"1100",         -- |          12       |  1  |  1  |  0  |  0  |
          '1'   when	"1101",         -- |          13       |  1  |  1  |  0  |  1  |
          '0'   when	"1110",         -- |          14       |  1  |  1  |  1  |  0  |
          '0'   when	"1111",         -- |          15       |  1  |  1  |  1  |  1  |
          '0'   when others;
   
   -- ------------------------------------------------------------------------------;
   -- ------------------------------------------------------------------------------
   -- |  fun�kn� hodnota f_g(d, c, b, a) |  stavov� index s  |  d  |  c  |  b  |  a  | 
   -- ------------------------------------------------------------------------------
   with input select
   f_g <= '1'   when	"0000",         -- |           0       |  0  |  0  |  0  |  0  |     
          '1'   when	"0001",         -- |           1       |  0  |  0  |  0  |  1  |
          '0'   when	"0010",         -- |           2       |  0  |  0  |  1  |  0  |
          '0'   when	"0011",         -- |           3       |  0  |  0  |  1  |  1  |
          '0'   when	"0100",         -- |           4       |  0  |  1  |  0  |  0  |
          '0'   when	"0101",         -- |           5       |  0  |  1  |  0  |  1  |
          '0'   when	"0110",         -- |           6       |  0  |  1  |  1  |  0  |
          '1'   when	"0111",         -- |           7       |  0  |  1  |  1  |  1  |
          '0'   when	"1000",         -- |           8       |  1  |  0  |  0  |  0  |
          '0'   when	"1001",         -- |           9       |  1  |  0  |  0  |  1  |
          '0'   when	"1010",         -- |          10       |  1  |  0  |  1  |  0  |
          '0'   when	"1011",         -- |          11       |  1  |  0  |  1  |  1  |
          '1'   when	"1100",         -- |          12       |  1  |  1  |  0  |  0  |
          '0'   when	"1101",         -- |          13       |  1  |  1  |  0  |  1  |
          '0'   when	"1110",         -- |          14       |  1  |  1  |  1  |  0  |
          '0'   when	"1111",         -- |          15       |  1  |  1  |  1  |  1  |
          '0'   when others;
    
   -- ------------------------------------------------------------------------------;
	
end decoder_7seg_arch;

