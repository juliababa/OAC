-- Package ( Pacote )
-- constantes e bibliotecas

library ieee;
use ieee.std_logic_1164.ALL;
use std.textio.all;
use ieee.numeric_std.all;

-- Entity (Entidade)
-- pinos de entrada e saída

entity testbench is end;
 
-- Architecture (Arquiterura)
-- implementações do projeto

architecture tbench of testbench is

 component genImm32 is
    port (
      instr : in std_logic_vector(31 downto 0);
      imm32 : out signed(31 downto 0)
      );
 end component;

-- Sinais auxiliares para a interconexao

signal inst :std_logic_vector(31 downto 0);
signal saida : signed(31 downto 0);

-- a definição se inicia por

begin
    DUT: genImm32 PORT MAP (inst, saida);

-- Implementacao do processo de estimulo

 estimulo: process
    begin 
        inst <= x"000002b3";
        wait for 1 ns;

        inst <= x"01002283";
        wait for 1 ns;

        inst <= x"f9c00313";
        wait for 1 ns;

        inst <= x"fff2c293";
        wait for 1 ns;

        inst <= x"16200313";
        wait for 1 ns;

        inst <= x"01800067";
        wait for 1 ns;

        inst <= x"00002437";
        wait for 1 ns;

        inst <= x"02542e23";
        wait for 1 ns;

        inst <= x"fe5290e3";
        wait for 1 ns;

        inst <= x"00c000ef";
        wait for 1 ns;

        inst <= 32x"0";

        wait;

-- Fim do processo de estimulo

    end process;

    monitor: process(inst)
        variable my_line : LINE;
    begin
        write(my_line, string'("saida="));
        write(my_line, saida);
        write(my_line, string'(",  at="));
        write(my_line, now);
        writeline(output, my_line);
    end process monitor;

--Fim da definiçao da arquitetura

end tbench;