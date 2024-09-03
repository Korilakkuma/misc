module m_ram();
  reg [31:0] mem [0:2];
  initial begin
    mem[0] <= 7;
    mem[1] <= 8;
    mem[2] <= 9;
  end
  wire [31:0] w_a = mem[0] + mem[1] + mem[2];
  initial #1 $display("%d %d %d %d", mem[0], mem[1], mem[2], w_a);
endmodule
