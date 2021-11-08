--  Organização e Arquitetura de Computadores - Turma C
--  TRABALHO FINAL: RISC-V Multiciclo
--  Aluno 1 : Júlia Yuri Garcia Baba - 190057921
--  Aluno 2: Gustavo Pereira Chaves - 190014113

--  IDE : Visual Studios Code
--  Compilador: Eda playground
--  Sistema operacional : Windows 10

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity XREGS is
  generic (WSIZE : natural := 32);
  port (
      clk, wren, rst : in std_logic;
      rs1, rs2, rd : in std_logic_vector(4 downto 0);
      data : in std_logic_vector(WSIZE-1 downto 0);
      ro1, ro2 : out std_logic_vector(WSIZE-1 downto 0)
    );
end XREGS;

architecture ARCH_XREGS of XREGS is

type reg is array (31 downto 0) of std_logic_vector (31 downto 0);

signal bank_reg :reg := (others => (others =>'0'));

begin
    process(clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                bank_reg <= (others => (others =>'0'));

            elsif wren = '1' then
                if to_integer(unsigned(rd)) /= 0 then
                    bank_reg(to_integer(unsigned(rd))) <= data;
                end if;
            end if;
            ro1 <= bank_reg(to_integer(unsigned(rs1)));
            ro2 <= bank_reg(to_integer(unsigned(rs2)));
        end if;
    end process;
end ARCH_XREGS;
    