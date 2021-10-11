library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;
use std.textio.all;


entity testbench is end;
 
architecture arq_tb of testbench is
 
    component XREGS is 
      generic (WSIZE : natural := 32);
      port (
        clk, wren, rst : in std_logic;
        rs1, rs2, rd : in std_logic_vector(4 downto 0);
        data : in std_logic_vector(WSIZE-1 downto 0);
        ro1, ro2 : out std_logic_vector(WSIZE-1 downto 0)
      );
    end component;
    
    signal clk  :std_logic := '0';
    signal wren :std_logic;
    signal rst :std_logic := '0';
    signal rs1, rs2, rd :std_logic_vector(4 downto 0) := 5x"0";
    signal ro1, ro2 :std_logic_vector(31 downto 0);
    signal data :std_logic_vector(31 downto 0);
   
  begin
    u0: XREGS PORT MAP (clk => clk, wren => wren, 
                        rst => rst, rs1 => rs1, 
                        rs2 => rs2, rd => rd, 
                        ro1 => ro1, ro2 => ro2,
                        data => data);

    clk <= not clk after 5 ns;

 estimulo: process
 	
    variable random: integer := 1;
    
    begin        
          wren <= '1';

          rd <= 5x"0";
          data <= 32x"1011";

          wait for 10 ns;

          wren <= '0';

          wait for 10 ns;

          assert(ro1 = 32x"0") report "ERROR ro1 | Reg = 0";
          assert(ro2 = 32x"0") report "ERROR ro2 | Reg = 0";

          for i in 1 to 31 loop
            wren <= '1';
            rd <= std_logic_vector(to_unsigned(i, 5));
            data <= std_logic_vector(to_unsigned(random, 32));
            
            wait for 10 ns;
            
            wren <= '0';

            rs1 <= std_logic_vector(to_unsigned(i, 5));
            rs2 <= std_logic_vector(to_unsigned(i, 5));
              
            wait for 10 ns;

            assert(ro1 = std_logic_vector(to_unsigned(random, 32))) report "ERROR ro1 | Reg = " & integer'image(i);
            assert(ro2 = std_logic_vector(to_unsigned(random, 32))) report "ERROR ro2 | Reg = " & integer'image(i);
            
            random := random + 5;

          end loop;
          
          -- Teste Reset
          rst <= '1';
          
          wait for 10 ns;
          
          rst <= '0';
          wren <= '0';
          
          for i in 0 to 31 loop
            rs1 <= std_logic_vector(to_unsigned(i, 5));
                        
            wait for 10 ns;
            
            assert(ro1 = 32x"0") report "ERROR ro1 | Reg = " & integer'image(i);
                        
          end loop;
          
        wait;
    end process;
    
end arq_tb;