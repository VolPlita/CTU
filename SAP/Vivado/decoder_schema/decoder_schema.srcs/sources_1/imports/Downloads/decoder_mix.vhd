library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


-- Popis vstupù a výstupù 7-segmentového dekodéru
--    vstupy: d, c, b, a
--    výstupy: f_a, f_b, f_c, f_d, f_e, f_f, f_g
-- výstupy f_a ... f_g ovládají jednotlivé segmenty displeje
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
-- popis chování 7-segmentového dekodéru
architecture decoder_7seg_arch of decoder_7seg is       
   -- vnitøní signál, který se používá k rozlišení jednotlivých vstupù
   signal   input :  std_logic_vector (3 downto 0);     
   
begin

   -- zpracování vstupù pro jednodušší práci s tabulkou 
   input(3) <= d;
   input(2) <= c;
   input(1) <= b;
   input(0) <= a;
	
-- Znázornìní 7-segmentového displeje
-- (více se mùžete dozvìdìt v referenèním manuálu pøípravku Basys-2)       
--       f_A  
--       --- 
--  f_F  |   |  f_B     
--       ---    <- f_G  
--  f_E  |   |  f_C                                    
--       ---                              
--       f_D
--
--  Každý segment je zapnutý (rozsvícený) hodnotou log. 0
--
--  Èísla 0..9 zobrazujte podle obecnì známé konvence,
--  pro ostatní hodnoty (A..F) použijte následující zobrazení:
--  A:      B:      C:      D:      E:      F:
--    ---             ---             ---     ---
--   |   |   |       |           |   |       |
--    ---     ---             ---     ---     ---
--   |   |   |   |   |       |   |   |       |
--            ---     ---     ---     ---

   -------------------------------------------------------------------
   -------------------------------------------------------------------
   -- Zde doplòte 3 algebraické výrazy pro segmenty CA, CB, CC      --
   -------------------------------------------------------------------
   -------------------------------------------------------------------
	
   f_a <= (not( ((not a) and (not c)) or (b and (not d)) or (b and c) or (a and c and (not d)) or ((not b) and (not c) and d) or ((not a) and c and d) ));
   f_b <= (not( ((not a) and (not c)) or ((not c) and (not d)) or ((not a) and (not b) and (not d)) or (a and b and (not d)) or (a and (not b) and d) ));
   f_c <= (not( ((not b) and (not d)) or (a and (not d)) or (a and (not b)) or (c and (not d)) or ((not c) and d) ));
	
   -------------------------------------------------------------------
   -------------------------------------------------------------------
   -- Zde doplòte výrazy tabulkou pro zbylé segmenty CD, CE, CF, CG --
   -------------------------------------------------------------------
   -------------------------------------------------------------------
	
   -- ------------------------------------------------------------------------------
   -- |  funèkní hodnota f_d(d, c, b, a) |  stavový index s  |  d  |  c  |  b  |  a  | 
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
   -- |  funèkní hodnota f_e(d, c, b, a) |  stavový index s  |  d  |  c  |  b  |  a  | 
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
   -- |  funèkní hodnota f_f(d, c, b, a) |  stavový index s  |  d  |  c  |  b  |  a  | 
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
   -- |  funèkní hodnota f_g(d, c, b, a) |  stavový index s  |  d  |  c  |  b  |  a  | 
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

