# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include <stdio.h>
# include <math.h>
# include "cap.h"

static PyObject* kmeans_pp(PyObject *self, PyObject *args)
{
    int k, N, vecdim, iter;
    double eps;
    double** vec_arr;
    double** centroids;
    
    /* This parses the Python arguments into:
        1. int (i) variables named k,N,vecdim,iter
        2. double (d) variable named eps
        3. A pointer to a pointer to a double (O!) variable named vec_arr
        4. A pointer to a pointer to a double (O!) variable named centroids */
    if(!PyArg_ParseTuple(args, "iiiidO!O!", &k, &N, &vecdim, &iter, &eps, &PyList_Type, &vec_arr, &PyList_Type, &centroids)) {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }
    /* Our function doesn't return value so we can don't need Py_BuildValue */
    kmeans(k, N, vecdim, iter, eps, vec_arr, centroids);
    Py_RETURN_NONE;
}

static PyMethodDef kmeansMethods[] = {
    {"kmeanspy",                   /* the Python method name that will be used */
      (PyCFunction) kmeans, /* the C-function that implements the Python function and returns static PyObject*  */
      METH_VARARGS,           /* flags indicating parameters accepted for this function */
      PyDoc_STR("kmeans clustering algorithim using kmeans++ as intial centroids")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as a
                                 sentinel. Python looks for this entry to know that all
                                 of the functions for the module have been defined. */
};

static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "kmeansmodule", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    kmeansMethods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_kmeansmodule(void)
{
    PyObject *m;
    m = PyModule_Create(&kmeansmodule);
    if (!m) {
        return NULL;
    }
    return m;
}

