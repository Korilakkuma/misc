module m_circuit_x1(w_clock);
  input wire w_clock;
  reg [31:0] r_pc = 0;
  wire [31:0] w_npc;
  m_adder adder (32'h4, r_pc, w_npc);
  always@(posedge w_clock) r_pc <= w_npc;
  wire [31:0] w_ir;
  m_async_imem amem (r_pc, w_ir);
endmodule

module m_test_circuit_x1();
  reg r_clock = 0;
  initial #150 forever #50 r_clock = ~r_clock;
  m_circuit_x1 m (r_clock);
  initial #99 forever #100 $display("%3d %h %h", $time, m.r_pc, m.w_ir);
  initial #400 $finish;
endmodule
