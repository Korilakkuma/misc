module m_set_associative_cache(w_clock, w_address, w_hit, w_dout, w_wa, w_we, w_wd);
  input wire w_clock, w_we;
  input wire [31:0] w_address;
  input wire [4:0] w_wa;
  input wire [57:0] w_wd;

  output wire w_hit;
  output wire [31:0] w_dout;

  wire w_hit1, w_hit2, w_we1, w_we2;
  wire [31:0] w_dout1, w_dout2;

  m_direct_mapped_cache way1 (w_clock, w_address, w_hit1, w_dout1, w_wa, w_we1, w_wd);
  m_direct_mapped_cache way2 (w_clock, w_address, w_hit2, w_dout2, w_wa, w_we2, w_wd);

  assign w_hit = w_hit1 | w_hit2;
  assign w_dout = w_hit1 ? w_dout1 : w_dout2;

  reg [0:0] lru [0:31];

  always @(posedge w_clock) if (w_hit) lru[w_address[6:2]] <= w_hit1;

  assign w_we1 = ((w_we & lru[w_wa]) == 0);
  assign w_we2 = ((w_we & lru[w_wa]) == 1);

  integer i;
  initial for (i = 0; i < 32; i = i + 1) lru[i] = 0;
endmodule
