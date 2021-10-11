library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;
use std.textio.all;

entity testbench is end;

architecture tb_arch of testbench is
    
    -- declaração do componente ULA
    component ulaRV is 
        generic (WSIZE : natural := 32);
        port (
            opcode : in std_logic_vector(3 downto 0);
            A, B : in std_logic_vector(WSIZE-1 downto 0);
            Z : out std_logic_vector(WSIZE-1 downto 0);
            cond : out std_logic
            );
    end component;

    -- declaração de sinais
    signal opcode: std_logic_vector(3 downto 0);
    signal A, B :std_logic_vector(31 downto 0);
    signal Z :std_logic_vector(31 downto 0);
    signal cond :std_logic;

    begin
    -- instanciação da ULA
        u0: ulaRV PORT MAP (
            opcode => opcode, A => A, 
            B => B, Z => Z, 
            cond => cond
        );

    --process
    estimulo: process
        begin
        -- ADD E SUB POSITIVO
            A <= 32x"6"; 
            B <= 32x"5";

            opcode <= "0000";
            wait for 1 ns;
            assert(Z = 32x"B") report "ERRO ADD POSITIVO";

            opcode <= "0001";
            wait for 1 ns;
            assert(Z = 32x"1") report "ERRO SUB POSITIVO";
       
       -- ADD E SUB NEGATIVOS
            A <= x"FFFFFFC8"; 
            B <= x"FFFFFFD9";

            opcode <= "0000";
            wait for 1 ns;
            assert(Z = x"FFFFFFA1") report "ERRO ADD NEGATIVO";

            opcode <= "0001";
            wait for 1 ns;
            assert(Z = x"FFFFFFEF") report "ERRO SUB NEGATIVO";

        -- ADD ZERO
            A <= x"FFFFFFF0";
            B <= 32x"10";
            
            opcode <= "0000";
            wait for 1 ns;
            assert(Z = 32x"0") report "ERRO ADD ZERO";
            
        -- SUB ZERO
        	A <= x"FFFFFFEC";
            B <= x"FFFFFFEC";
            
            opcode <= "0001";
            wait for 1 ns;
            assert(Z = 32x"0") report "ERRO SUB ZERO";             
                     
        -- AND
            A <= 32x"1220";
            B <= 32x"1AE8";
            
            opcode <= "0010";
            wait for 1 ns;
            assert(Z = 32x"1220") report "ERRO AND";

        -- OR
            A <= 32x"1DE9";
            B <= 32x"2672";
            
            opcode <= "0011";
            wait for 1 ns;
            assert(Z = 32x"3FFB") report "ERRO OR";
        
        -- XOR
            A <= 32x"1DF2";
            B <= 32x"2624";
            
            opcode <= "0100";
            wait for 1 ns;
            assert(Z = 32x"3BD6") report "ERRO XOR";
        
        -- SLL
        	A <= 32x"1DDA";
            B <= 32x"2";
            
            opcode <= "0101";
            wait for 1 ns;
            assert(Z = 32x"7768") report "ERRO SLL";
        
        -- SRL
        	A <= 32x"19BC";
            B <= 32x"3";
            
            opcode <= "0110";
            wait for 1 ns;
            assert(Z = 32x"337") report "ERRO SRL";
       
       -- SRA
        	A <= 32x"E211";
            B <= 32x"2";
            
            opcode <= "0111";
            wait for 1 ns;
            assert(Z = 32x"3884") report "ERRO SRL";
       
       -- SLT
        	A <= x"FFFFFFC9";
            B <= x"FFFFFFD3";
            
            opcode <= "1000";
            wait for 1 ns;
            assert(cond = '1') report "ERRO SLT";
        
       -- SLTU 
       		A <= 32x"15"; 
            B <= 32x"E21A";
               	            
            opcode <= "1001";
            wait for 1 ns;
            assert(cond = '1') report "ERRO SLTU";
            
      -- SGE
      		A <= x"FFFFFF92";
            B <= x"FFFFFF2D";
               	            
            opcode <= "1010";
            wait for 1 ns;
            assert(cond = '1') report "ERRO SGE";
      
      -- SGEU
      		A <= x"FFFFE21A"; 
            B <= 32x"15";
               	            
            opcode <= "1011";
            wait for 1 ns;
            assert(cond = '1') report "ERRO SGEU";
            
      -- SEQ
      		A <= 32x"3E";
            B <= 32x"3E";
               	            
            opcode <= "1100";
            wait for 1 ns;
            assert(cond = '1') report "ERRO SEQ";
            
      -- SNE
      		A <= 32x"3";
            B <= 32x"3E";
               	            
            opcode <= "1101";
            wait for 1 ns;
            assert(cond = '1') report "ERRO SNE";
            	
            wait;
        end process;

end tb_arch;