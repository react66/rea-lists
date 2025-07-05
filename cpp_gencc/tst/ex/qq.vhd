library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity qq is
port( nreset,clk: in std_logic;
);
end qq;

architecture rtl of qq is

begin

process (nreset,clk)
begin
if (nreset='0') then
elsif (clk'event and clk='1') then
end if;
end process;

end rtl;
