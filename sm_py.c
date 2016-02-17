#include <Python.h>
#include "sm.h"
#include "hexString.h"

#define BUFFER_SIZE 512

static char module_docstring[] = "This module provides an interface to OTR's implementation of the socialist millionaire protocol";
static char sm_key_init_docstring[] = "Intitializes keys used in socialist millionare protocol";
static char sm_step_1_docstring[] = "Socialist Millionaire Protocol step 1";
static char sm_step_2_docstring[] = "Socialist Millionaire Protocol step 2";
static char sm_step_3_docstring[] = "Socialist Millionaire Protocol step 3";
static char sm_step_4_docstring[] = "Socialist Millionaire Protocol step 4";
static char sm_step_5_docstring[] = "Socialist Millionaire Protocol step 5";
static char sm_step_6_docstring[] = "Socialist Millionaire Protocol step 6";

PyObject * sm_py_sm_key_init(PyObject * self, PyObject * args);
PyObject * sm_py_sm_step_1(PyObject * self, PyObject * args);
PyObject * sm_py_sm_step_2(PyObject * self, PyObject * args);
PyObject * sm_py_sm_step_3(PyObject * self, PyObject * args);
PyObject * sm_py_sm_step_4(PyObject * self, PyObject * args);
PyObject * sm_py_sm_step_5(PyObject * self, PyObject * args);
PyObject * sm_py_sm_step_6(PyObject * self, PyObject * args);

static PyMethodDef module_methods[] = {
    {"key_init", sm_py_sm_key_init, METH_VARARGS, sm_key_init_docstring},
    {"step_1", sm_py_sm_step_1, METH_VARARGS, sm_step_1_docstring},
    {"step_2", sm_py_sm_step_2, METH_VARARGS, sm_step_2_docstring},    
    {"step_3", sm_py_sm_step_3, METH_VARARGS, sm_step_3_docstring},    
    {"step_4", sm_py_sm_step_4, METH_VARARGS, sm_step_4_docstring},    
    {"step_5", sm_py_sm_step_5, METH_VARARGS, sm_step_5_docstring},    
    {"step_6", sm_py_sm_step_6, METH_VARARGS, sm_step_6_docstring},    
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initsm_py(void)
{
    PyObject * m = Py_InitModule3("sm_py", module_methods, module_docstring);
    if (m == NULL)
    {  
        return;
    }
}

void mpiToString(gcry_mpi_t mpi, unsigned char * buffer, size_t bufLen)
{
    if(mpi != NULL)
    {
        gcry_mpi_print(GCRYMPI_FMT_HEX, buffer, bufLen, NULL, mpi);
        
        if((strlen(buffer) <= 2) && (strcmp(buffer, "02") != 0)
)
        {
            sprintf((char *)buffer, "");
        }
    }
    else
    {
        sprintf((char *)buffer, "");
    }
}

void stringToMpi(gcry_mpi_t * mpi, unsigned char * buffer)
{
    unsigned long length = strlen((char *)buffer);
    if((length > 2) || (strcmp(buffer, "02") == 0))
    {
        gcry_mpi_scan(mpi, GCRYMPI_FMT_HEX, buffer, 0, NULL);
    }
    else
    {
        *mpi = gcry_mpi_new(get_sm_mod_len_bits());
    }
}

PyObject * sm_py_sm_key_init(PyObject * self, PyObject * args)
{
    otrl_sm_init();
    OtrlSMState state;
    otrl_sm_state_new(&state);
    otrl_sm_state_init(&state);
    
    unsigned char * secretString = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x2String = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x3String = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g1String = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g2String = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3String = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3oString = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pString = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qString = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pabString = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qabString = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));

    mpiToString(state.secret, secretString, BUFFER_SIZE);
    mpiToString(state.x2, x2String, BUFFER_SIZE);    
    mpiToString(state.x3, x3String, BUFFER_SIZE);    
    mpiToString(state.g1, g1String, BUFFER_SIZE);    
    mpiToString(state.g2, g2String, BUFFER_SIZE);    
    mpiToString(state.g3, g3String, BUFFER_SIZE);    
    mpiToString(state.g3o, g3oString, BUFFER_SIZE);    
    mpiToString(state.p, pString, BUFFER_SIZE);
    mpiToString(state.q, qString, BUFFER_SIZE);        
    mpiToString(state.pab, pabString, BUFFER_SIZE);    
    mpiToString(state.qab, qabString, BUFFER_SIZE);   
 
    PyObject * ret = Py_BuildValue("{s:(sssssssssss)}","keys", secretString, x2String, x3String, g1String, g2String, g3String, g3oString, pString, qString, pabString, qabString);

    free(secretString);
    free(x2String);
    free(x3String);
    free(g1String);
    free(g2String);
    free(g3String);
    free(g3oString);
    free(pString);
    free(qString);
    free(pabString);
    free(qabString);

    return ret;
}

PyObject * sm_py_sm_step_1(PyObject * self, PyObject * args)
{
    otrl_sm_init();
    OtrlSMState state;
    otrl_sm_state_new(&state);
 
    unsigned char * secretString;
    unsigned char * x2String;
    unsigned char * x3String;
    unsigned char * g1String;
    unsigned char * g2String;
    unsigned char * g3String;
    unsigned char * g3oString;
    unsigned char * pString;
    unsigned char * qString;
    unsigned char * pabString;
    unsigned char * qabString;
    unsigned char * newSecretString;

    if(!PyArg_ParseTuple(args, "s(sssssssssss)", &newSecretString, &secretString, &x2String, &x3String, &g1String, &g2String, &g3String, &g3oString, &pString, &qString, &pabString, &qabString))
    {
        return NULL;
    }

    stringToMpi(&(state.secret), secretString);
    stringToMpi(&(state.x2), x2String);
    stringToMpi(&(state.x3), x3String);
    stringToMpi(&(state.g1), g1String);
    stringToMpi(&(state.g2), g2String);
    stringToMpi(&(state.g3), g3String);
    stringToMpi(&(state.g3o), g3oString);
    stringToMpi(&(state.p), pString);
    stringToMpi(&(state.q), qString);
    stringToMpi(&(state.pab), pabString);
    stringToMpi(&(state.qab), qabString);

    int outputLength;
    unsigned char * output = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));

    otrl_sm_step1(&state, newSecretString, strlen(newSecretString), &output, &outputLength);
    
    unsigned char * outputString;
    outputString = bytesToHexString(output, outputLength);

    free(output);

    unsigned char * secretStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g1StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3oStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));

    mpiToString(state.secret, secretStringOutput, BUFFER_SIZE);
    mpiToString(state.x2, x2StringOutput, BUFFER_SIZE);    
    mpiToString(state.x3, x3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g1, g1StringOutput, BUFFER_SIZE);    
    mpiToString(state.g2, g2StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3, g3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3o, g3oStringOutput, BUFFER_SIZE);    
    mpiToString(state.p, pStringOutput, BUFFER_SIZE);
    mpiToString(state.q, qStringOutput, BUFFER_SIZE);        
    mpiToString(state.pab, pabStringOutput, BUFFER_SIZE);    
    mpiToString(state.qab, qabStringOutput, BUFFER_SIZE);  

    otrl_sm_state_free(&state);    
    
    PyObject * ret = Py_BuildValue("{s:i,s:s,s:(sssssssssss)}","messageLength", outputLength, "message", outputString, "keys", secretStringOutput, x2StringOutput, x3StringOutput, g1StringOutput, g2StringOutput, g3StringOutput, g3oStringOutput, pStringOutput, qStringOutput, pabStringOutput, qabStringOutput);


    free(secretStringOutput);
    free(x2StringOutput);
    free(x3StringOutput);
    free(g1StringOutput);
    free(g2StringOutput);
    free(g3StringOutput);
    free(g3oStringOutput);
    free(pStringOutput);
    free(qStringOutput);
    free(pabStringOutput);
    free(qabStringOutput);
    free(outputString);

    return ret;
}

PyObject * sm_py_sm_step_2(PyObject * self, PyObject * args)
{
    otrl_sm_init();
    OtrlSMState state;
    otrl_sm_state_new(&state);
 
    unsigned char * secretString;
    unsigned char * x2String;
    unsigned char * x3String;
    unsigned char * g1String;
    unsigned char * g2String;
    unsigned char * g3String;
    unsigned char * g3oString;
    unsigned char * pString;
    unsigned char * qString;
    unsigned char * pabString;
    unsigned char * qabString;
    unsigned char * input;

    if(!PyArg_ParseTuple(args, "s(sssssssssss)", &input, &secretString, &x2String, &x3String, &g1String, &g2String, &g3String, &g3oString, &pString, &qString, &pabString, &qabString))
    {
        return NULL;
    }

    int inputLength = strlen(input) / 2;

    stringToMpi(&(state.secret), secretString);
    stringToMpi(&(state.x2), x2String);
    stringToMpi(&(state.x3), x3String);
    stringToMpi(&(state.g1), g1String);
    stringToMpi(&(state.g2), g2String);
    stringToMpi(&(state.g3), g3String);
    stringToMpi(&(state.g3o), g3oString);
    stringToMpi(&(state.p), pString);
    stringToMpi(&(state.q), qString);
    stringToMpi(&(state.pab), pabString);
    stringToMpi(&(state.qab), qabString);

    unsigned char * inputBytes;
    inputBytes = hexStringToBytes(input);

    otrl_sm_step2a(&state, inputBytes, inputLength, 1);

    free(inputBytes);
    
    unsigned char * secretStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g1StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3oStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));

    mpiToString(state.secret, secretStringOutput, BUFFER_SIZE);
    mpiToString(state.x2, x2StringOutput, BUFFER_SIZE);    
    mpiToString(state.x3, x3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g1, g1StringOutput, BUFFER_SIZE);    
    mpiToString(state.g2, g2StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3, g3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3o, g3oStringOutput, BUFFER_SIZE);    
    mpiToString(state.p, pStringOutput, BUFFER_SIZE);
    mpiToString(state.q, qStringOutput, BUFFER_SIZE);        
    mpiToString(state.pab, pabStringOutput, BUFFER_SIZE);    
    mpiToString(state.qab, qabStringOutput, BUFFER_SIZE); 

    otrl_sm_state_free(&state);    
    
    PyObject * ret = Py_BuildValue("{s:(sssssssssss)}","keys", secretStringOutput, x2StringOutput, x3StringOutput, g1StringOutput, g2StringOutput, g3StringOutput, g3oStringOutput, pStringOutput, qStringOutput, pabStringOutput, qabStringOutput);

    free(secretStringOutput);
    free(x2StringOutput);
    free(x3StringOutput);
    free(g1StringOutput);
    free(g2StringOutput);
    free(g3StringOutput);
    free(g3oStringOutput);
    free(pStringOutput);
    free(qStringOutput);
    free(pabStringOutput);
    free(qabStringOutput);

    return ret;
}

PyObject * sm_py_sm_step_3(PyObject * self, PyObject * args)
{
    otrl_sm_init();
    OtrlSMState state;
    otrl_sm_state_new(&state);
 
    unsigned char * secretString;
    unsigned char * x2String;
    unsigned char * x3String;
    unsigned char * g1String;
    unsigned char * g2String;
    unsigned char * g3String;
    unsigned char * g3oString;
    unsigned char * pString;
    unsigned char * qString;
    unsigned char * pabString;
    unsigned char * qabString;
    unsigned char * newSecretString;

    if(!PyArg_ParseTuple(args, "s(sssssssssss)", &newSecretString, &secretString, &x2String, &x3String, &g1String, &g2String, &g3String, &g3oString, &pString, &qString, &pabString, &qabString))
    {
        return NULL;
    }

    stringToMpi(&(state.secret), secretString);
    stringToMpi(&(state.x2), x2String);
    stringToMpi(&(state.x3), x3String);
    stringToMpi(&(state.g1), g1String);
    stringToMpi(&(state.g2), g2String);
    stringToMpi(&(state.g3), g3String);
    stringToMpi(&(state.g3o), g3oString);
    stringToMpi(&(state.p), pString);
    stringToMpi(&(state.q), qString);
    stringToMpi(&(state.pab), pabString);
    stringToMpi(&(state.qab), qabString);

    int outputLength;
    unsigned char * output = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));

    otrl_sm_step2b(&state, newSecretString, strlen(newSecretString), &output, &outputLength);

    unsigned char * outputString;
    outputString = bytesToHexString(output, outputLength);
    free(output);
    
    unsigned char * secretStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g1StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3oStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));

    mpiToString(state.secret, secretStringOutput, BUFFER_SIZE);
    mpiToString(state.x2, x2StringOutput, BUFFER_SIZE);    
    mpiToString(state.x3, x3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g1, g1StringOutput, BUFFER_SIZE);    
    mpiToString(state.g2, g2StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3, g3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3o, g3oStringOutput, BUFFER_SIZE);    
    mpiToString(state.p, pStringOutput, BUFFER_SIZE);
    mpiToString(state.q, qStringOutput, BUFFER_SIZE);        
    mpiToString(state.pab, pabStringOutput, BUFFER_SIZE);    
    mpiToString(state.qab, qabStringOutput, BUFFER_SIZE); 

    otrl_sm_state_free(&state);     
    
    PyObject * ret = Py_BuildValue("{s:s,s:(sssssssssss)}","message", outputString, "keys", secretStringOutput, x2StringOutput, x3StringOutput, g1StringOutput, g2StringOutput, g3StringOutput, g3oStringOutput, pStringOutput, qStringOutput, pabStringOutput, qabStringOutput);

    free(secretStringOutput);
    free(x2StringOutput);
    free(x3StringOutput);
    free(g1StringOutput);
    free(g2StringOutput);
    free(g3StringOutput);
    free(g3oStringOutput);
    free(pStringOutput);
    free(qStringOutput);
    free(pabStringOutput);
    free(qabStringOutput);
    free(outputString);

    return ret;
}

PyObject * sm_py_sm_step_4(PyObject * self, PyObject * args)
{
    otrl_sm_init();
    OtrlSMState state;
    otrl_sm_state_new(&state);
 
    unsigned char * secretString;
    unsigned char * x2String;
    unsigned char * x3String;
    unsigned char * g1String;
    unsigned char * g2String;
    unsigned char * g3String;
    unsigned char * g3oString;
    unsigned char * pString;
    unsigned char * qString;
    unsigned char * pabString;
    unsigned char * qabString;
    unsigned char * input;

    if(!PyArg_ParseTuple(args, "s(sssssssssss)", &input, &secretString, &x2String, &x3String, &g1String, &g2String, &g3String, &g3oString, &pString, &qString, &pabString, &qabString))
    {
        return NULL;
    }

    int inputLength = strlen(input) / 2;

    stringToMpi(&(state.secret), secretString);
    stringToMpi(&(state.x2), x2String);
    stringToMpi(&(state.x3), x3String);
    stringToMpi(&(state.g1), g1String);
    stringToMpi(&(state.g2), g2String);
    stringToMpi(&(state.g3), g3String);
    stringToMpi(&(state.g3o), g3oString);
    stringToMpi(&(state.p), pString);
    stringToMpi(&(state.q), qString);
    stringToMpi(&(state.pab), pabString);
    stringToMpi(&(state.qab), qabString);

    unsigned char * output = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    int outputLength;
    
    unsigned char * inputBytes;
    inputBytes = hexStringToBytes(input);

    otrl_sm_step3(&state, inputBytes, inputLength, &output, &outputLength);

    free(inputBytes);
    
    unsigned char * outputString;
    outputString = bytesToHexString(output, outputLength);
    free(output);
    
    unsigned char * secretStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g1StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3oStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));

    mpiToString(state.secret, secretStringOutput, BUFFER_SIZE);
    mpiToString(state.x2, x2StringOutput, BUFFER_SIZE);    
    mpiToString(state.x3, x3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g1, g1StringOutput, BUFFER_SIZE);    
    mpiToString(state.g2, g2StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3, g3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3o, g3oStringOutput, BUFFER_SIZE);    
    mpiToString(state.p, pStringOutput, BUFFER_SIZE);
    mpiToString(state.q, qStringOutput, BUFFER_SIZE);        
    mpiToString(state.pab, pabStringOutput, BUFFER_SIZE);    
    mpiToString(state.qab, qabStringOutput, BUFFER_SIZE); 

    otrl_sm_state_free(&state);    
    
    PyObject * ret = Py_BuildValue("{s:s,s:(sssssssssss)}", "message", outputString, "keys",  secretStringOutput, x2StringOutput, x3StringOutput, g1StringOutput, g2StringOutput, g3StringOutput, g3oStringOutput, pStringOutput, qStringOutput, pabStringOutput, qabStringOutput);

    free(secretStringOutput);
    free(x2StringOutput);
    free(x3StringOutput);
    free(g1StringOutput);
    free(g2StringOutput);
    free(g3StringOutput);
    free(g3oStringOutput);
    free(pStringOutput);
    free(qStringOutput);
    free(pabStringOutput);
    free(qabStringOutput);
    free(outputString);

    return ret;
}

PyObject * sm_py_sm_step_5(PyObject * self, PyObject * args)
{
    otrl_sm_init();
    OtrlSMState state;
    otrl_sm_state_new(&state);
 
    unsigned char * secretString;
    unsigned char * x2String;
    unsigned char * x3String;
    unsigned char * g1String;
    unsigned char * g2String;
    unsigned char * g3String;
    unsigned char * g3oString;
    unsigned char * pString;
    unsigned char * qString;
    unsigned char * pabString;
    unsigned char * qabString;
    unsigned char * input;

    if(!PyArg_ParseTuple(args, "s(sssssssssss)", &input, &secretString, &x2String, &x3String, &g1String, &g2String, &g3String, &g3oString, &pString, &qString, &pabString, &qabString))
    {
        return NULL;
    }

    int inputLength = strlen(input) / 2;

    stringToMpi(&(state.secret), secretString);
    stringToMpi(&(state.x2), x2String);
    stringToMpi(&(state.x3), x3String);
    stringToMpi(&(state.g1), g1String);
    stringToMpi(&(state.g2), g2String);
    stringToMpi(&(state.g3), g3String);
    stringToMpi(&(state.g3o), g3oString);
    stringToMpi(&(state.p), pString);
    stringToMpi(&(state.q), qString);
    stringToMpi(&(state.pab), pabString);
    stringToMpi(&(state.qab), qabString);

    unsigned char * output = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    int outputLength;

    unsigned char * inputBytes;
    inputBytes = hexStringToBytes(input);

    gcry_error_t error = otrl_sm_step4(&state, inputBytes, inputLength, &output, &outputLength);

    free(inputBytes);

    unsigned char * outputString;
    outputString = bytesToHexString(output, outputLength);
    free(output);
    
    int success = 0;

    if (error == GPG_ERR_NO_ERROR)
    {
        success = 1;
    }
    
    unsigned char * secretStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g1StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3oStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));

    mpiToString(state.secret, secretStringOutput, BUFFER_SIZE);
    mpiToString(state.x2, x2StringOutput, BUFFER_SIZE);    
    mpiToString(state.x3, x3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g1, g1StringOutput, BUFFER_SIZE);    
    mpiToString(state.g2, g2StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3, g3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3o, g3oStringOutput, BUFFER_SIZE);    
    mpiToString(state.p, pStringOutput, BUFFER_SIZE);
    mpiToString(state.q, qStringOutput, BUFFER_SIZE);        
    mpiToString(state.pab, pabStringOutput, BUFFER_SIZE);    
    mpiToString(state.qab, qabStringOutput, BUFFER_SIZE); 

    otrl_sm_state_free(&state);    
    
    PyObject * ret = Py_BuildValue("{s:i,s:s,s:(sssssssssss)}","success", success, "message", outputString, "keys", secretStringOutput, x2StringOutput, x3StringOutput, g1StringOutput, g2StringOutput, g3StringOutput, g3oStringOutput, pStringOutput, qStringOutput, pabStringOutput, qabStringOutput);

    free(secretStringOutput);
    free(x2StringOutput);
    free(x3StringOutput);
    free(g1StringOutput);
    free(g2StringOutput);
    free(g3StringOutput);
    free(g3oStringOutput);
    free(pStringOutput);
    free(qStringOutput);
    free(pabStringOutput);
    free(qabStringOutput);
    free(outputString);

    return ret;
}

PyObject * sm_py_sm_step_6(PyObject * self, PyObject * args)
{
    otrl_sm_init();
    OtrlSMState state;
    otrl_sm_state_new(&state);
 
    unsigned char * secretString;
    unsigned char * x2String;
    unsigned char * x3String;
    unsigned char * g1String;
    unsigned char * g2String;
    unsigned char * g3String;
    unsigned char * g3oString;
    unsigned char * pString;
    unsigned char * qString;
    unsigned char * pabString;
    unsigned char * qabString;
    unsigned char * input;

    if(!PyArg_ParseTuple(args, "s(sssssssssss)", &input, &secretString, &x2String, &x3String, &g1String, &g2String, &g3String, &g3oString, &pString, &qString, &pabString, &qabString))
    {
        return NULL;
    }

    int inputLength = strlen(input) / 2;

    stringToMpi(&(state.secret), secretString);
    stringToMpi(&(state.x2), x2String);
    stringToMpi(&(state.x3), x3String);
    stringToMpi(&(state.g1), g1String);
    stringToMpi(&(state.g2), g2String);
    stringToMpi(&(state.g3), g3String);
    stringToMpi(&(state.g3o), g3oString);
    stringToMpi(&(state.p), pString);
    stringToMpi(&(state.q), qString);
    stringToMpi(&(state.pab), pabString);
    stringToMpi(&(state.qab), qabString);

    unsigned char * inputBytes;
    inputBytes = hexStringToBytes(input);

    gcry_error_t error = otrl_sm_step5(&state, inputBytes, inputLength);

    free(inputBytes);
    
    int success = 0;

    if (error == GPG_ERR_NO_ERROR)
    {
        success = 1;
    }
    
    unsigned char * secretStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * x3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g1StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g2StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3StringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * g3oStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * pabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));
    unsigned char * qabStringOutput = (unsigned char *) malloc(BUFFER_SIZE * sizeof(unsigned char));

    mpiToString(state.secret, secretStringOutput, BUFFER_SIZE);
    mpiToString(state.x2, x2StringOutput, BUFFER_SIZE);    
    mpiToString(state.x3, x3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g1, g1StringOutput, BUFFER_SIZE);    
    mpiToString(state.g2, g2StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3, g3StringOutput, BUFFER_SIZE);    
    mpiToString(state.g3o, g3oStringOutput, BUFFER_SIZE);    
    mpiToString(state.p, pStringOutput, BUFFER_SIZE);
    mpiToString(state.q, qStringOutput, BUFFER_SIZE);        
    mpiToString(state.pab, pabStringOutput, BUFFER_SIZE);    
    mpiToString(state.qab, qabStringOutput, BUFFER_SIZE); 

    otrl_sm_state_free(&state);   
    
    PyObject * ret = Py_BuildValue("{s:i,s:(sssssssssss)}","success", success,"keys",secretStringOutput, x2StringOutput, x3StringOutput, g1StringOutput, g2StringOutput, g3StringOutput, g3oStringOutput, pStringOutput, qStringOutput, pabStringOutput, qabStringOutput);

    free(secretStringOutput);
    free(x2StringOutput);
    free(x3StringOutput);
    free(g1StringOutput);
    free(g2StringOutput);
    free(g3StringOutput);
    free(g3oStringOutput);
    free(pStringOutput);
    free(qStringOutput);
    free(pabStringOutput);
    free(qabStringOutput);

    return ret;
}


