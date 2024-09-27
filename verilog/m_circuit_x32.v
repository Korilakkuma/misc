module m_circuit_x32(w_clock);
  input wire w_clock;

  wire [31:0] w_npc;
  wire [31:0] w_ir;
  wire [31:0] w_r1, w_r2;
  wire [31:0] w_rt;

  reg [31:0] r_pc = 0;
  reg [31:0] r_x1 = 3;

  m_adder adder_if (32'h4, r_pc, w_npc);

  assign w_ir = (r_pc == 0) ? { 7'd0, 5'd2, 5'd1, 3'd0, 5'd5, 7'h33 } :  // add x5, x1, x2
                (r_pc == 4) ? { 7'd0, 5'd4, 5'd3, 3'd0, 5'd6, 7'h33 } :  // add x6, x3, x4
                              { 7'd0, 5'd6, 5'd5, 3'd0, 5'd7, 7'h33 };   // add x7, x5, x6

  m_RF rf_id (w_clock, w_ir[19:15], w_ir[24:20], w_r1, w_r2, w_ir[11:7], 1'b1, w_rt);

  m_adder adder_ex (w_r1, w_r2, w_rt);

  always@(posedge w_clock) r_pc <= w_npc;
endmodule

module m_test_circuit_x32();
  reg r_clock = 0;
  initial #150 forever #50 r_clock = ~r_clock;

  m_circuit_x32 m (r_clock);

  initial m.rf_id.mem[1] = 5;
  initial m.rf_id.mem[2] = 6;
  initial m.rf_id.mem[3] = 7;
  initial m.rf_id.mem[4] = 8;

  initial #99 forever #100 $display("%3d %h %d %d %d", $time, m.r_pc, m.w_r1, m.w_r2, m.w_rt);
  initial #400 $finish;
endmodule
