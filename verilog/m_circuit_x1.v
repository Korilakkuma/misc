module m_circuit_x1(w_clock);
  input wire w_clock;

  wire [31:0] w_npc;
  wire [31:0] w_ir;
  wire [31:0] w_r1, w_r2;
  wire [31:0] w_rt;

  wire w_cmp_rs1, w_cmp_rs2;

  reg [31:0] r_pc = 0;
  reg [31:0] r_x1 = 3;

  m_adder adder_if (32'h4, r_pc, w_npc);
  m_async_imem if_amem (r_pc, w_ir);

  m_comparator comparator_id_rs1 (5'd1, w_ir[19:15], w_cmp_rs1);
  m_comparator comparator_id_rs2 (5'd1, w_ir[24:20], w_cmp_rs2);

  m_mux multiplexer_r1 (32'h0, r_x1, w_cmp_rs1, w_r1);
  m_mux multiplexer_r2 (32'h0, r_x1, w_cmp_rs2, w_r2);

  m_adder adder_ex (w_r1, w_r2, w_rt);

  always@(posedge w_clock) r_pc <= w_npc;
  always@(posedge w_clock) r_x1 <= w_rt;
endmodule

module m_test_circuit_x1();
  reg r_clock = 0;
  initial #150 forever #50 r_clock = ~r_clock;
  m_circuit_x1 m (r_clock);
  initial #99 forever #100 $display("%3d 32'h%1d %d %d %d", $time, m.r_pc, m.w_r1, m.w_r2, m.w_rt);
  initial #400 $finish;
endmodule
