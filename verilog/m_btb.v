module m_btb(w_clock, w_pc, w_hit, w_dout, w_wa, w_we, w_wd);
  input wire w_clock, w_we;
  input wire [31:0] w_pc;
  input wire [4:0] w_wa;
  input wire [57:0] w_wd;

  output wire w_hit;
  output wire [31:0] w_dout;

  wire w_v;
  wire [24:0] w_tag;
  wire [31:0] w_data;

  reg [57:0] mem [0:31];

  always @(posedge w_clock) if (w_we) mem[w_wa] <= w_wd;

  assign { w_v, w_tag, w_data } = mem[w_pc[6:2]];

  assign w_hit  = w_v & (w_tag == w_pc[31:7]);
  assign w_dout = w_data;

  integer i;
  initial for (i = 0; i < 32; i = i + 1) mem[i] = 0;
endmodule
