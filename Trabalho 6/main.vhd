--  Organização e Arquitetura de Computadores - Turma C
--  TRABALHO 6: Projeto e Simulação de uma ULA em VHDL
--  Aluno(a): Júlia Yuri Garcia Baba
--  Matrícula: 190057921

--  IDE : Visual Studios Code
--  Compilador: Eda playground
--  Sistema operacional : Windows 10

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity ulaRV is
    generic (WSIZE : natural := 32);
    port (
          opcode : in std_logic_vector(3 downto 0);
          A, B : in std_logic_vector(WSIZE-1 downto 0);
          Z : out std_logic_vector(WSIZE-1 downto 0);
          cond : out std_logic
        );
end ulaRV;


architecture arch_ulaRV of ulaRV is
begin
    process(opcode, A, B, Z, cond)
    begin
        case opcode is

            -- ADD 
            when "0000" =>
            Z <= std_logic_vector(signed(A) + signed(B));

            -- SUB
            when "0001" =>
            Z <= std_logic_vector(signed(A) - signed(B));

            -- AND
            when "0010" =>
            Z <= A AND B;
            -- OR
            when "0011" =>
            Z <= A OR B;

            -- XOR
            when "0100" =>
            Z <= A XOR B;

            -- SLL
            when "0101" =>
            Z <= std_logic_vector(shift_left(unsigned(A), to_integer(unsigned(B))));

            -- SRL
            when "0110" =>
            Z <=  std_logic_vector(shift_right(unsigned(A), to_integer(unsigned(B))));

            -- SRA
            when "0111" =>
            Z <=  std_logic_vector(shift_right(signed(A), to_integer(unsigned(B))));

            -- SLT
            when "1000" =>
            if signed(A) < signed(B) then
                cond <= '1';
            else
            	cond <= '0';
            end if;

            -- SLTU
            when "1001" =>
            if unsigned(A) < unsigned(B) then
                cond <= '1';
            else
            	cond <= '0';
            end if;

            -- SGE
            when "1010" =>
            if signed(A) >= signed(B) then
                cond <= '1';
            else
            	cond <= '0';
            end if;

            -- SGEU
            when "1011" =>
            if unsigned(A) >= unsigned(B) then
                cond <= '1';
			else
            	cond <= '0';
            end if;

            -- SEQ
            when "1100" =>
            if A = B then
                cond <= '1';
            else
            	cond <= '0';
            end if;

            -- SNE
            when "1101" =>
            if A /= B then
                cond <= '1';
            else
            	cond <= '0';
            end if;

            when others => Z <= 32x"404";
        end case;
    end process;
end arch_ulaRV;
    