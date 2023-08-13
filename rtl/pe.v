module pe(
    input clk,
    input rst_n,

    input [15:0] a,
    input [15:0] b,
    output [31:0] out
);

reg [31:0] sum;
assign out = sum;
always @(posedge clk or negedge rst_n) begin
    if(~rst_n) begin
        sum <= 0;
    end
    else begin
        sum <= sum + a * b;
    end
end

endmodule