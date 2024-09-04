module m_include();
  reg [31:0] mem [0:2];
  `include "ram.v"
  wire [31:0] w_a = mem[0] + mem[1] + mem[2];
  initial #1 $display("%d %d %d %d", mem[0], mem[1], mem[2], w_a);
endmodule
