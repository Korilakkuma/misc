module m_direct_mapped_multiword_cache(w_clock, w_address, w_hit, w_dout, w_wa, w_we, w_wd);
  input wire w_clock, w_we;
  input wire [31:0] w_address;
  input wire [4:0] w_wa;
  input wire [88:0] w_wd;

  output wire w_hit;
  output wire [31:0] w_dout;

  wire w_v;
  wire [23:0] w_tag;
  wire [31:0] w_d1, w_d2;

  reg [88:0] mem [0:31];

  always @(posedge w_clock) if (w_we) mem[w_wa] <= w_wd;

  assign { w_v, w_tag, w_d1, w_d2 } = mem[w_address[7:3]];
  assign w_hit = w_v & (w_tag == w_address[31:8]);
  assign w_dout = w_address[2] ? w_d1 : w_d2;

  integer i;
  initial for (i = 0; i < 32; i = i + 1) mem[i] = 0;
endmodule
