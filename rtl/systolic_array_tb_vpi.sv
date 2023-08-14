
module systolic_array_tb_vpi;

  // Parameters

  //Ports
  reg  clk;
  reg  rst_n;
  reg [4*16-1:0] a;
  reg [4*16-1:0] b;
  wire [16*32-1:0] result;

  systolic_array  systolic_array_inst (
                    .clk(clk),
                    .rst_n(rst_n),
                    .a(a),
                    .b(b),
                    .result(result)
                  );

  //always #5  clk = ! clk ;


  task print_matrix_16;
    input [16*16-1:0]matix;
    integer  i,j;
    begin
      for (i = 0; i < 4; i = i + 1)
      begin
        $write("[");
        for (j = 0; j < 4; j = j + 1)
        begin
          $write("%d ", matix[(4*i+j)*16-1-:16]);
        end
        $write("]\n");
      end
    end
  endtask


  task print_matrix_32;
    input [16*32-1:0]matix;
    integer  i,j;
    begin
      for (i = 0; i < 4; i = i + 1)
      begin
        $write("[");
        for (j = 0; j < 4; j = j + 1)
        begin
          $write("%d ", matix[(4*i+j+1)*32-1-:32]);
        end
        $write("]\n");
      end
    end
  endtask


  initial
  begin
    clk = 0;
    forever
      #5 clk = ! clk;
  end

  initial
  begin
    rst_n = 0;
    $initialize_matrix();
    #10 rst_n = 1;
    $display("start test");
  end


  initial
  begin
    wait(rst_n == 1'b1); 
    // #30;
    for (integer step= 0; step<2*4; step=step+1)
    begin
      $display("step = %d",step);
      @(posedge clk);
      a = $get_matrix_a(step);
      b = $get_matrix_b(step);
      print_matrix_32(result);
    end
    repeat(4) @(posedge clk);
    $display("result:");
    print_matrix_32(result);
    $finish;
  end

  initial
  begin
    $dumpfile("systolic_array.vcd");
    $dumpvars(0, systolic_array_tb_vpi);
  end
endmodule
