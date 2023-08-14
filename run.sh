#!/bin/bash

rtl_path="./rtl"
tb_file="$rtl_path/systolic_array_tb.sv"
echo $src_files

file_name=$(basename "$tb_file")
top_module=${file_name%.*}
echo "top module: ${top_module}" 

iverilog -g2012 -s $top_module -y$rtl_path -o sim $tb_file
vvp -n sim -lxt2
