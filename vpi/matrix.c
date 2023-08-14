#include <vpi_user.h>
#include <sv_vpi_user.h>

static int matrix_a[4][4];
static int matrix_b[4][4];

void initialize_matrix_calltf() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix_a[i][j] = i*4+j+1;
            matrix_b[i][j] = i*4+j+1;
        }
    }
}


PLI_INT32 get_matrix_a_calltf(PLI_BYTE8 *user_data) {
    vpiHandle systfref, args_iter, argh;
    s_vpi_value val;
    systfref = vpi_handle(vpiSysTfCall, NULL);
    args_iter = vpi_iterate(vpiArgument, systfref);
    argh = vpi_scan(args_iter);
    
    
    val.format = vpiIntVal;  // We expect an integer
    vpi_get_value(argh, &val);
    int step = val.value.integer;
    
    s_vpi_vecval result[2]; // for 64-bit value
    result[0].aval = 0;
    result[0].bval = 0;
    result[1].aval = 0;
    result[1].bval = 0;
    
    for(int j=0; j<4; j++){
        int temp = - step + 3 + j;
        if(temp >= 0 && temp <= 3){
            if (j<=2) {
                result[0].aval = result[0].aval | (matrix_a[temp][j] << (j*16));
            } else {
                result[1].aval = result[1].aval | (matrix_a[temp][j] << ((j-2)*16));
            }
        }
    }

    val.format = vpiVectorVal;  // Setting format to vector since it's a 64-bit value
    val.value.vector = result;  // Assuming suitable conversion here
    vpi_put_value(systfref, &val, NULL, vpiNoDelay);
    
    return 0;  // Typically return 0 for success
}


PLI_INT32 get_matrix_b_calltf(PLI_BYTE8 *user_data) {
    vpiHandle systfref, args_iter, argh;
    s_vpi_value val;
    systfref = vpi_handle(vpiSysTfCall, NULL);
    args_iter = vpi_iterate(vpiArgument, systfref);
    argh = vpi_scan(args_iter);
    
    
    val.format = vpiIntVal;  // We expect an integer
    vpi_get_value(argh, &val);
    int step = val.value.integer;
    
    s_vpi_vecval result[2]; // for 64-bit value
    result[0].aval = 0;
    result[0].bval = 0;
    result[1].aval = 0;
    result[1].bval = 0;
    
    for(int i=0; i<4; i++){
        int temp = - step + 3 + i;
        if(temp >= 0 && temp <= 3){
            if (i<=2) {
                result[0].aval = result[0].aval | (matrix_a[i][temp] << (i*16));
            } else {
                result[1].aval = result[1].aval | (matrix_a[i][temp] << ((i-2)*16));
            }
        }
    }

    val.format = vpiVectorVal;  // Setting format to vector since it's a 64-bit value
    val.value.vector = result;  // Assuming suitable conversion here
    vpi_put_value(systfref, &val, NULL, vpiNoDelay);
    
    return 0;  // Typically return 0 for success
}

void register_systolic_array_vpi() {
    s_vpi_systf_data tf_data_init = {
        vpiSysTask,
        0,
        "$initialize_matrix",
        initialize_matrix_calltf, 
        0, 
        0, 
        0};
    vpi_register_systf(&tf_data_init);

    s_vpi_systf_data tf_data_ga = {
        vpiSysFunc,     // Indicating this is a system function
        vpiSizedFunc,  // The function returns a register vector
        "$get_matrix_a",// Name of the function in Verilog
        get_matrix_a_calltf, // The C function to be called
        NULL, // user_data, can be kept as NULL
        NULL, // tf, can be kept as NULL
        NULL  // sizetf, can be kept as NUL
    };
    vpi_register_systf(&tf_data_ga);

    s_vpi_systf_data tf_data_gb = {
        vpiSysFunc,     // Indicating this is a system function
        vpiSizedFunc,  // The function returns a register vector
        "$get_matrix_b",// Name of the function in Verilog
        get_matrix_b_calltf, // The C function to be called
        NULL, // user_data, can be kept as NULL
        NULL, // tf, can be kept as NULL
        NULL  // sizetf, can be kept as NUL
    };
    vpi_register_systf(&tf_data_gb);
}

void (*vlog_startup_routines[])() = {
    register_systolic_array_vpi,
    0
};
