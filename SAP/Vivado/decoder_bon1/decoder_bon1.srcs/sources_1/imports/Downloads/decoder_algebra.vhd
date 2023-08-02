-- Required libraries
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Ports definition
entity decoder_7seg is
	port(
		a   : in  STD_LOGIC;
		b   : in  STD_LOGIC;
		c   : in  STD_LOGIC;
		d   : in  STD_LOGIC;
		f_a : out STD_LOGIC;
		f_b : out STD_LOGIC;
		f_c : out STD_LOGIC;
		f_d : out STD_LOGIC;
		f_e : out STD_LOGIC;
		f_f : out STD_LOGIC;
		f_g : out STD_LOGIC
	);
end decoder_7seg;


-- Definition of the behavior - by algebraic expressions
architecture Behavioral of decoder_7seg is
begin

-- Add the respective excitation functions here.
-- The functions are negated here, because the display segments switch on with log. 0
-- Example:
--   f_g <= (not(((not a) and b) or (b and (not c)) or ((not c) and d) or ((not b) and c and (not d))));
	
	f_a <= (not( ((not a) and (not c)) or (b and (not d)) or (b and c) or (a and c and (not d)) or ((not b) and (not c) and d) or ((not a) and c and d) ));
	f_b <= (not( ((not a) and (not c)) or ((not c) and (not d)) or ((not a) and (not b) and (not d)) or (a and b and (not d)) or (a and (not b) and d) ));
	f_c <= (not( ((not b) and (not d)) or (a and (not d)) or (a and (not b)) or (c and (not d)) or ((not c) and d) ));
	f_d <= (not( ((not b) and d) or ((not a) and (not c) and (not d)) or (a and b and (not c)) or (a and (not b) and c) or ((not a) and b and c) ));
	f_e <= (not( ((not a) and b) or (c and d) or (b and d) or ((not a) and (not c)) ));
	f_f <= (not( ((not a) and (not b)) or ((not c) and d) or (b and d) or ((not b) and c and (not d)) or ((not a) and b and c) ));
	f_g <= (not( ((not c) and d) or ((not a) and b) or (b and (not c)) or (a and d) or ((not b) and c and (not d)) ));

end Behavioral; 
