`define D_DELAY 5
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

module m_simulator(w_clock, w_cc);
  input wire w_clock;
  input wire [31:0] w_cc;

  reg [31:0] r_pc = 0;
  reg        r_re = 0;

  wire [31:0] w_insn;
  wire w_oe;

  m_msx_dram m (w_clock, r_pc, r_re, w_insn, w_oe);

  initial begin
    m.mem[0] = 11;
    m.mem[1] = 22;
    m.mem[2] = 33;
    m.mem[3] = 44;

    #280 r_pc <= 4; r_re <= 1;
    #100 r_pc <= 0; r_re <= 0;
    #500 r_pc <= 8; r_re <= 1;
    #100 r_pc <= 0; r_re <= 0;
  end

  initial #99 forever #100 $display("CC%02d %3d %3d %3d %3d %3d", w_cc, m.w_pc, m.w_re, m.r_c, m.r_insn, m.r_oe);
  initial #1450 $finish;
endmodule

module m_main();
  reg r_clock = 0;
  initial #150 forever #50 r_clock = ~r_clock;

  reg [31:0] r_cc = 1;
  always@(posedge r_clock) r_cc <= r_cc + 1;

  initial #1000000 begin
    $display("Timeout");
    $finish;
  end

  m_simulator m (r_clock, r_cc);

  initial $dumpvars(0, m);
endmodule
