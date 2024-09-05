module m_for();
  reg [31:0] mem [0:1023];
  integer i;
  initial for (i = 0; i < 1024; i = i + 1) mem[i] = 0;
  initial mem[0] <= 7;
  wire [31:0] w_a = mem[0] + mem[1] + mem[2];
  initial #1 $display("%d %d %d %d", mem[0], mem[1], mem[2], w_a);
endmodule
