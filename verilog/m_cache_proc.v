`define D_DELAY 3
module m_simulator(w_clock, w_cc);
  input wire w_clock;
  input wire [31:0] w_cc;

  wire w_re, w_oe;
  wire [31:0] w_pc, w_ir;

  m_cache_proc proc (w_clock, w_pc, w_ir, w_oe, w_re);
  m_msx_dram dram (w_clock, w_pc, w_re, w_ir, w_oe);

  initial begin
    `define MEM dram.mem
    `include "asm.txt"
  end

  initial #99 forever #100 $display("CC%02d %h %h %h %h %b %5d %5d %5d", w_cc, proc.r_pc, proc.P1_pc, proc.P2_pc, proc.P3_pc, proc.w_stall, proc.w_in1, proc.w_in2, proc.w_alu);
endmodule

module m_cache_proc(w_clock, r_pc, w_ir_in, w_oe, w_re);
  input wire w_clock, w_oe;
  input wire [31:0] w_ir_in;

  output reg [31:0] r_pc = 0;
  output wire w_re;

  reg [31:0] P1_ir = 32'h13;
  reg [31:0] P1_pc = 0;
  reg [31:0] P2_pc = 0;
  reg [31:0] P3_pc = 0;

  reg [31:0] P2_r1 = 0, P2_s2 = 0, P2_r2 = 0, P2_tpc = 0;
  reg [31:0] P3_alu = 0, P3_ldd = 0;

  reg P2_r = 0, P2_s = 0, P2_b = 0, P2_ld = 0, P3_s = 0, P3_b = 0, P3_ld = 0;

  reg [4:0] P2_rd = 0, P2_rs1 = 0, P2_rs2 = 0, P3_rd = 0;

  reg P1_v = 0, P2_v = 0, P3_v = 0;

  wire [31:0] w_npc, w_imm, w_r1, w_r2, w_s2, w_rt;
  wire [31:0] w_alu, w_ldd, w_tpc, w_pcin, w_in1, w_in2, w_in3;

  wire w_r, w_i, w_s, w_b, w_u, w_j, w_ld, w_token;

  wire w_miss = P2_b & w_token & P2_v;

  wire w_hit;
  wire [31:0] w_dout;
  wire [57:0] w_wd = { 1'b1, r_pc[31:7], w_ir_in };

  m_direct_mapped_cache cache (w_clock, r_pc, w_hit, w_dout, r_pc[6:2], w_oe, w_wd);

  wire [31:0] w_ir = w_hit ? w_dout : w_ir_in;
  wire w_stall = !w_hit;

  assign w_re = !w_hit;

  m_mux multiplexer_if (w_npc, P2_tpc, w_miss, w_pcin);
  m_adder adder_if (32'h4, r_pc, w_npc);
  // m_async_imem imem_if (r_pc, w_ir);

  m_gen_imm imm_id (P1_ir, w_imm, w_r, w_i, w_s, w_b, w_u, w_j, w_ld);
  m_RF rf_id (w_clock, P1_ir[19:15], P1_ir[24:20], w_r1, w_r2, P3_rd, (!P3_s & !P3_b & P3_v & !w_stall), w_rt);
  m_adder adder_id (w_imm, P1_pc, w_tpc);
  m_mux multiplexer_id (w_r2, w_imm, (!w_r & !w_b), w_s2);

  always @(posedge w_clock) if (!w_stall) begin
    { P1_v, P2_v, P3_v } <= { !w_miss, (!w_miss & P1_v), P2_v };
    { r_pc, P1_ir, P1_pc, P2_pc } <= { w_pcin, w_ir, r_pc, P1_pc };
    { P2_r1, P2_r2, P2_s2, P2_tpc } <= { w_r1, w_r2, w_s2, w_tpc };
    { P2_r, P2_s, P2_b, P2_ld } <= { w_r, w_s, w_b, w_ld };
    { P2_rs2, P2_rs1, P2_rd } <= { P1_ir[24:20], P1_ir[19:15], P1_ir[11:7] };
    { P3_pc, P3_s, P3_b, P3_ld } <= { P2_pc, P2_s, P2_b, P2_ld };
    { P3_alu, P3_ldd, P3_rd } <= { w_alu, w_ldd, P2_rd };
  end

  m_alu alu_ex (w_in1, w_in2, w_alu, w_token);

  m_async_data_mem mem_ma (w_clock, w_alu, (P2_s & P2_v & !w_stall), w_in3, w_ldd);

  m_mux multiplexer_wb (P3_alu, P3_ldd, P3_ld, w_rt);

  wire w_f = !P3_s & !P3_b & | P3_rd & P3_v;

  m_mux multiplexer_wb_pipe1 (P2_r1, w_rt, (w_f & (P2_rs1 == P3_rd)), w_in1);
  m_mux multiplexer_wb_pipe2 (P2_s2, w_rt, (w_f & (P2_rs2 == P3_rd) & (P2_r | P2_b)), w_in2);
  m_mux multiplexer_wb_pipe3 (P2_r2, w_rt, (w_f & (P2_rs2 == P3_rd)), w_in3);
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

module m_alu(w_in1, w_in2, w_out, w_token);
  input  wire [31:0] w_in1, w_in2;
  output wire [31:0] w_out;
  output wire w_token;

  assign w_out   = w_in1 + w_in2;
  assign w_token = w_in1 != w_in2;
endmodule

module m_msx_dram(w_clock, w_pc, w_re, r_insn, r_oe);
  input wire w_clock, w_re;
  input wire [31:0] w_pc;

  output reg [31:0] r_insn = 0;
  output reg r_oe = 0;

  reg [31:0] mem [0:2047];  // 32 bits (1 word) x 2048 = 65536 bits = 8192 bytes = 8 MiB
  reg [31:0] r_c = 1, r_pc = 0;

  always @(posedge w_clock) begin
    r_pc <= ((r_c == 1) & w_re) ? w_pc : r_pc;
    r_c <= ((r_c == 1) & w_re) ? 2 : ((r_c == 1) | (r_c == `D_DELAY)) ? 1 : (r_c + 1);
    r_oe <= (r_c == `D_DELAY - 1);
    r_insn <= (r_c == `D_DELAY - 1) ? mem[r_pc[12:2]] : 0;
  end

  integer i;
  initial for (i = 0; i < 2048; i = i + 1) mem[i] = 32'd0;
endmodule
