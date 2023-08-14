#include <vpi_user.h>
#include <sv_vpi_user.h>
#include <stdint.h>

static uint16_t matrix_a[4][4];
static uint16_t matrix_b[4][4];

void initialize_matrix_calltf() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix_a[i][j] = i*4+j+1;
            matrix_b[i][j] = i*4+j+1;
        }
    }

    vpi_printf("Matrix A:\n");
    print_matrix(matrix_a);
    vpi_printf("Matrix B:\n");
    print_matrix(matrix_b);
}


void print_matrix(int matrix[4][4]) {
    for (int i = 0; i < 4; i++) {
        vpi_printf("[%d\t%d\t%d\t%d]\n", matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3]);
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
    
    // s_vpi_vecval result[2]; // for 64-bit value
    p_vpi_vecval result = (p_vpi_vecval)malloc(sizeof(s_vpi_vecval)*2);
    result[0].aval = 0;
    result[0].bval = 0;
    result[1].aval = 0;
    result[1].bval = 0;
    
    for(int j=0; j<4; j++){
        int temp = - step + 3 + j;
        if(temp >= 0 && temp <= 3){
            if (j<2) {
                result[0].aval  |= (matrix_a[temp][j] << (j*16));
            } else {
                result[1].aval  |= (matrix_a[temp][j] << ((j-2)*16));
            }
        }
    }
    vpi_printf("a:result[0].aval = %08x\n", result[0].aval);
    vpi_printf("a:result[1].aval = %08x\n", result[1].aval);

    val.format = vpiVectorVal;  // Setting format to vector since it's a 64-bit value
    val.value.vector = result;  // Assuming suitable conversion here
    vpi_put_value(systfref, &val, NULL, vpiNoDelay);
    
    free(result);

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
    // vpi_printf("step = %d\n", step);
    
    // s_vpi_vecval result[2]; // for 64-bit value
    p_vpi_vecval result = (p_vpi_vecval)malloc(sizeof(s_vpi_vecval)*2);
    result[0].aval = 0;
    result[0].bval = 0;
    result[1].aval = 0;
    result[1].bval = 0;
    
    for(int i=0; i<4; i++){
        int temp = - step + 3 + i;
        if(temp >= 0 && temp <= 3){
            if (i<2) {
                result[0].aval  |= (matrix_b[i][temp] << (i*16));
            } else {
                result[1].aval  |= (matrix_b[i][temp] << ((i-2)*16));    
            }
        }
    }
    vpi_printf("b:result[0].aval = %08x\n", result[0].aval);
    vpi_printf("b:result[1].aval = %08x\n", result[1].aval);

    val.format = vpiVectorVal;  // Setting format to vector since it's a 64-bit value
    val.value.vector = result;  // Assuming suitable conversion here
    vpi_put_value(systfref, &val, NULL, vpiNoDelay);
    
    free(result);

    return 0;  // Typically return 0 for success
}


PLI_INT32 sized_function_64(PLI_BYTE8 *user_data) {
    return 64; 
}


void register_systolic_array_vpi() {
    s_vpi_systf_data *tf_data_ptr = (s_vpi_systf_data *)malloc(sizeof(s_vpi_systf_data));
    if (!tf_data_ptr) {
        // Handle memory allocation failure
        return;
    }

    // Register $initialize_matrix
    *tf_data_ptr = (s_vpi_systf_data){
        .type           = vpiSysTask,
        .sysfunctype    = vpiSysTask,
        .tfname         = "$initialize_matrix",
        .calltf         = initialize_matrix_calltf,
        .compiletf      = NULL,
        .sizetf         = NULL,
        .user_data      = NULL
    };
    vpi_register_systf(tf_data_ptr);

    // Register $get_matrix_a
    *tf_data_ptr = (s_vpi_systf_data){
        .type           = vpiSysFunc,
        .sysfunctype    = vpiSizedFunc,
        .tfname         = "$get_matrix_a",
        .calltf         = get_matrix_a_calltf,
        .compiletf      = NULL,
        .sizetf         = sized_function_64,
        .user_data      = NULL
    };
    vpi_register_systf(tf_data_ptr);

    // Register $get_matrix_b
    *tf_data_ptr = (s_vpi_systf_data){
        .type           = vpiSysFunc,
        .sysfunctype    = vpiSizedFunc,
        .tfname         = "$get_matrix_b",
        .calltf         = get_matrix_b_calltf,
        .compiletf      = NULL,
        .sizetf         = sized_function_64,
        .user_data      = NULL
    };
    vpi_register_systf(tf_data_ptr);

    free(tf_data_ptr); // Release the dynamically allocated memory
}


void (*vlog_startup_routines[])() = {
    register_systolic_array_vpi,
    0
};
