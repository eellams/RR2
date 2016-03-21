// From the python tutorial https://docs.python.org/3/extending/embedding.html
// usage: gcc -I/usr/include/python3.4 emb.c -o emb -lpython3.4m

// Don't forget to import "pythonXX.lib"!
#include <Python.h>

static int numargs=0;

/* Return the number of arguments of the application command line */
static PyObject*
emb_numargs(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return PyLong_FromLong(numargs);
}

static PyMethodDef EmbMethods[] = {
    {"numargs", emb_numargs, METH_VARARGS,
     "Return the number of arguments received by the process."},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef EmbModule = {
    PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,
    NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_emb(void)
{
    return PyModule_Create(&EmbModule);
}

int main(int argc, char *argv[])
{
  /*wchar_t *program = Py_DecodeLocale(argv[0], NULL);
  if (program == NULL) {
    fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
    exit(1);
  }
  Py_SetProgramName(program);*/  /* optional but recommended */

  numargs = argc;
  PyImport_AppendInittab("emb", &PyInit_emb);
  Py_Initialize();

  PyRun_SimpleString("import emb; print(\"Number of arguments\", emb.numargs())");
  Py_Finalize();
  //PyMem_RawFree(program);
  return 0;
}
