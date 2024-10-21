module m_simulator(w_clock, w_cc);
  input wire w_clock;
  input wire [31:0] w_cc;

  m_circuit_5_stages_pipe m (w_clock);

  initial begin
    `define MEM m.imem_if.mem
    `include "asm.txt"
  end

  initial #99 forever #100 $display("CC%1d %h %h %h %h %d %d %d", w_cc, m.r_pc, m.P1_pc, m.P2_pc, m.P3_pc, m.w_in1, m.w_in2, m.w_alu);
endmodule

module m_circuit_5_stages_pipe(w_clock);
  input wire w_clock;

  reg [31:0] P1_ir = 32'h13;
  reg [31:0] P1_pc = 0;
  reg [31:0] P2_pc = 0;
  reg [31:0] P3_pc = 0;
  reg [31:0] P4_pc = 0;

  reg [31:0] P2_r1 = 0, P2_s2 = 0, P2_r2 = 0, P2_tpc = 0, P3_r2 = 0;
  reg [31:0] P3_alu, P3_in3;
  reg [31:0] P4_alu = 0, P4_ldd = 0;

  reg P2_r = 0, P2_s = 0, P2_b = 0, P2_ld = 0, P4_s = 0, P4_b = 1, P4_ld = 0;
  reg P3_s = 0, P3_b = 0, P3_ld = 0;

  reg [4:0] P2_rd = 0, P2_rs1 = 0, P2_rs2 = 0, P3_rd = 0, P4_rd = 0;

  reg P1_v = 0, P2_v = 0, P3_v = 0;

  wire [31:0] w_npc, w_ir, w_imm, w_r1, w_r2, w_s2, w_rt;
  wire [31:0] w_alu, w_ldd, w_tpc, w_pcin, w_in1, w_in2, w_in3;

  wire w_r, w_i, w_s, w_b, w_u, w_j, w_ld, w_token;

  reg [31:0] r_pc = 0;

  wire w_miss = P2_b & w_token & P2_v;

  m_mux multiplexer_if (w_npc, P2_tpc, w_miss, w_pcin);
  m_adder adder_if (32'h4, r_pc, w_npc);
  m_async_imem imem_if (r_pc, w_ir);

  m_gen_imm imm_id (P1_ir, w_imm, w_r, w_i, w_s, w_b, w_u, w_j, w_ld);
  m_RF rf_id (w_clock, P1_ir[19:15], P1_ir[24:20], w_r1, w_r2, P3_rd, (!P3_s & !P3_b & P3_v), w_rt);
  m_adder adder_id (w_imm, P1_pc, w_tpc);
  m_mux multiplexer_id (w_r2, w_imm, (!w_r & !w_b), w_s2);

  always @(posedge w_clock) begin
    { P1_v, P2_v, P3_v } <= { !w_miss, (!w_miss & P1_v), P2_v };
    { r_pc, P1_ir, P1_pc, P2_pc } <= { w_pcin, w_ir, r_pc, P1_pc };
    { P2_r1, P2_r2, P2_s2, P2_tpc } <= { w_r1, w_r2, w_s2, w_tpc };
    { P2_r, P2_s, P2_b, P2_ld } <= { w_r, w_s, w_b, w_ld };
    { P2_rs2, P2_rs1, P2_rd } <= { P1_ir[24:20], P1_ir[19:15], P1_ir[11:7] };
    { P3_pc, P3_ld, P3_r2, P3_in3 } <= { P2_pc, P2_ld, P2_r2, w_in3 };
    { P3_alu, P3_rd } <= { w_alu, P2_rd };
    { P3_s, P3_b, P3_ld } <= { P2_s, P2_b, P2_ld };
    { P4_pc, P4_s, P4_b, P4_ld } <= { P3_pc, P3_s, P3_b, P3_ld };
    { P4_alu, P4_ldd, P4_rd } <= { w_alu, w_ldd, P3_rd };
  end

  m_alu alu_ex (w_in1, w_in2, w_alu, w_token);

  m_async_data_mem mem_ma (w_clock, P3_alu, P3_s, P3_in3, w_ldd);

  m_mux multiplexer_wb (P4_alu, P4_ldd, P4_ld, w_rt);

  wire w_f3 = !P3_s & !P3_b & | P3_rd;
  wire w_f4 = !P4_s & !P4_b & | P4_rd;

  wire w_forward1_P3 = (w_f3 & (P3_rd == P2_rs1));
  wire w_forward1_P4 = (w_f4 & (P4_rd == P2_rs1));
  wire w_forward2_P3 = (w_f3 & (P3_rd == P2_rs2) & (P2_r | P2_b));
  wire w_forward2_P4 = (w_f4 & (P4_rd == P2_rs2) & (P2_r | P2_b));
  wire w_forward3_P3 = (w_f3 & (P3_rd == P2_rs1));
  wire w_forward3_P4 = (w_f4 & (P4_rd == P2_rs1));

  assign w_in1 = w_forward1_P3 ? P3_alu : (w_forward1_P4 ? w_rt : P2_r1);
  assign w_in2 = w_forward2_P3 ? P3_alu : (w_forward2_P4 ? w_rt : P2_s2);
  assign w_in3 = w_forward3_P3 ? P3_alu : (w_forward3_P4 ? w_rt : P2_r2);
endmodule

module m_instruction_type(opcode5, r, i, s, b, u, j);
  input wire [4:0] opcode5;
  output wire r, i, s, b, u, j;
  assign j = (opcode5 == 5'b11011);
  assign b = (opcode5 == 5'b11000);
  assign s = (opcode5 == 5'b01000);
  assign r = (opcode5 == 5'b01100);
  assign u = ((opcode5 == 5'b01101) || (opcode5 == 5'b00101));
  assign i = ~(j | b | s | r | u);
endmodule

module m_get_immediate(ir, i, s, b, u, j, imm);
  input wire [31:0] ir;
  input wire i, s, b, u, j;
  output wire [31:0] imm;
  assign imm = i ? { { 20{ ir[31] } }, ir[31:20] } :
               s ? { { 20{ ir[31] } }, ir[31:25], ir[11:7] } :
               b ? { { 20{ ir[31] } }, ir[7], ir[30:25], ir[11:8], 1'b0 } :
               u ? { ir[31:12], 12'b0 } :
               j ? { { 12{ ir[31] } }, ir[19:12], ir[20], ir[30:21], 1'b0 } : 0;
endmodule

module m_adder(w_in1, w_in2, w_out);
  input wire [31:0] w_in1, w_in2;
  output wire [31:0] w_out;
  assign w_out = w_in1 + w_in2;
endmodule

module m_mux(w_in1, w_in2, w_s, w_out);
  input wire [31:0] w_in1, w_in2;
  input wire w_s;
  output wire [31:0] w_out;
  assign w_out = w_s ? w_in2 : w_in1;
endmodule

module m_RF(w_clock, w_ra1, w_ra2, w_rd1, w_rd2, w_wa, w_we, w_wd);
  input wire w_clock, w_we;
  input wire [4:0] w_ra1, w_ra2, w_wa;

  output wire [31:0] w_rd1, w_rd2;

  input wire [31:0] w_wd;

  reg [31:0] mem [0:31];

  wire w_bp1 = (w_we & (w_ra1 == w_wa));
  wire w_bp2 = (w_we & (w_ra2 == w_wa));

  assign w_rd1 = (w_ra1 == 5'd0) ? 32'd0 : (w_bp1 ? w_wd : mem[w_ra1]);
  assign w_rd2 = (w_ra2 == 5'd0) ? 32'd0 : (w_bp2 ? w_wd : mem[w_ra2]);

  always @(posedge w_clock) if (w_we) mem[w_wa] <= w_wd;
  always @(posedge w_clock) if (w_we & (w_wa == 5'd30)) $finish;

  integer i;
  initial for (i = 0; i < 32; i = i + 1) mem[i] = 0;
endmodule

module m_gen_imm(w_ir, w_imm, w_r, w_i, w_s, w_b, w_u, w_j, w_ld);
  input  wire [31:0] w_ir;
  output wire [31:0] w_imm;
  output wire w_r, w_i, w_s, w_b, w_u, w_j, w_ld;

  m_instruction_type instruction_type (w_ir[6:2], w_r, w_i, w_s, w_b, w_u, w_j);
  m_get_immediate get_immediate (w_ir, w_i, w_s, w_b, w_u, w_j, w_imm);

  assign w_ld = (w_ir[6:2] == 0);
endmodule

module m_async_imem(w_pc, w_insn);
  input  wire [31:0] w_pc;
  output wire [31:0] w_insn;

  reg [31:0] mem [0:63];

  assign w_insn = mem[w_pc[7:2]];

  integer i;

  initial for (i = 0; i < 64; i = i + 1) mem[i] = 32'd0;
endmodule

module m_sync_imem(w_clock, w_pc, r_insn);
  input wire w_clock;
  input wire [31:0] w_pc;

  output reg [31:0] r_insn;

  reg [31:0] mem [0:63];

  always @(posedge w_clock) r_insn <= mem[w_pc[7:2]];

  integer i;

  initial for (i = 0; i < 64; i = i + 1) mem[i] = 32'd0;
endmodule

module m_async_data_mem(w_clock, w_address, w_we, w_wd, w_rd);
  input  wire w_clock, w_we;
  input  wire [31:0] w_address, w_wd;
  output wire [31:0] w_rd;

  reg [31:0] mem [0:63];

  assign w_rd = mem[w_address[7:2]];

  always @(posedge w_clock) if (w_we) mem[w_address[7:2]] <= w_wd;

  integer i;
  initial for (i = 0; i < 64; i = i + 1) mem[i] = 32'd0;
endmodule

module m_sync_data_mem(w_clock, w_address, w_we, w_wd, r_rd);
  input  wire w_clock, w_we;
  input  wire [31:0] w_address, w_wd;

  output reg [31:0] r_rd;

  reg [31:0] mem [0:63];

  always @(posedge w_clock) r_rd <= mem[w_address[7:2]];
  always @(posedge w_clock) if (w_we) mem[w_address[7:2]] <= w_wd;

  integer i;
  initial for (i = 0; i < 64; i = i + 1) mem[i] = 32'd0;
endmodule

module m_alu(w_in1, w_in2, w_out, w_token);
  input  wire [31:0] w_in1, w_in2;
  output wire [31:0] w_out;
  output wire w_token;

  assign w_out   = w_in1 + w_in2;
  assign w_token = w_in1 != w_in2;
endmodule
