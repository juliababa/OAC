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

--Entity (Entidade)
--pinos de entrada e saída

entity genImm32 is
  port (
    instr : in std_logic_vector(31 downto 0);
    imm32 : out signed(31 downto 0)
  );
end genImm32;

--Architecture (Arquitetura)
--implementacoes do projeto

architecture genImm32_arq of genImm32 is

    type FORMAT_RV is (R_type, I_type, S_type, SB_type, UJ_type, U_type);

    signal opcodes :FORMAT_RV;

-- a definicao inicia por 

begin
  process(instr, opcodes)
  begin
    case (instr(6 downto 0)) is
        when 7x"33" =>    opcodes <= R_Type;
        when 7x"03" | 7x"13"| 7x"67" => opcodes <= I_Type; 
        when 7x"23" => opcodes <= S_Type;
        when 7x"63" => opcodes <= SB_Type;
        when 7x"37" | 7x"17" => opcodes <= U_Type;
        when 7x"6F" => opcodes <= UJ_Type;
        when others =>        
       end case;
  end process;
  
  process(instr, opcodes)
  begin
      case opcodes is

          when R_Type => imm32 <= resize("0", 32);
          
          when I_Type => imm32 <= resize(signed(instr(31 downto 20)), 32);
          
          when S_Type => imm32 <= resize(signed(instr(31 downto 25) & instr(11 downto 7)), 32);
          
          when SB_Type => imm32 <= resize(signed(instr(31 downto 31) & instr(7 downto 7) & instr(30 downto 25) & instr(11 downto 8)), 32);
          
          when U_Type => imm32 <= signed(instr(31 downto 12) & 12B"0");
          
          when UJ_Type => imm32 <= resize(signed(instr(31 downto 31) & instr(19 downto 12) & instr(20 downto 20) & instr(30 downto 21)), 32);

          when others =>
         
          end case;

  end process;

-- fim da definição

END;