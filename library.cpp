
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <unordered_map>

#include <phpcpp.h>


wchar_t *py_program;

//std::unordered_map<std::string, std::string> modulesMap;
std::unordered_map<std::string, PyObject *> moduleRefMap;

PyObject *jsonModule, *jsonDumps, *jsonLoads;

/**
 *  PyClass class that can be used for counting
 */
class PyClass : public Php::Base {
private:
    /**
     *  The internal value
     *  @var    int
     */
//    int _value = 0;
    PyObject * _currentModule;

public:
    /**
     *  C++ constructor and destructor
     */
    PyClass() {}

    virtual ~PyClass() {}

    /**
     *  php "constructor"
     *  @param  params
     */
    void __construct(Php::Parameters &params) {
        std::string key = params[0];
        auto match = moduleRefMap.find(key);
        if (match != moduleRefMap.end()) {
            _currentModule = match->second;
            return;
        }
        if (params.size() >=3) {
            std::string initScript = params[2];
            PyRun_SimpleString(initScript.c_str());
        }

        std::string _moduleName = params[1];
        _currentModule = PyImport_ImportModule(_moduleName.c_str());
        std::pair<std::string, PyObject *> newModule(params[0],_currentModule);
        moduleRefMap.insert(newModule);
    };

    Php::Value __call(const char *name, Php::Parameters &params)
    {
        PyObject *_currentFunction, *funcRes;
        std::string jsonArgs("[]"), jsonRes;
        PyObject *pArgs, *jsonArgsPyObj, *jsonResPyObj;
        _currentFunction = PyObject_GetAttrString(_currentModule, name);

        Php::Value jsonEncodeArgs, jsonDecodeArgs;
        jsonArgs = std::string(Php::call("json_encode",params));
        if (_currentFunction && PyCallable_Check(_currentFunction)) {
            // Decode JSON
            pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(jsonArgs.c_str()));
            jsonArgsPyObj = PyObject_CallObject(jsonLoads, pArgs);

//            Py_DECREF(pArgs);
            // Execute call
            pArgs = PySequence_Tuple(jsonArgsPyObj);
            funcRes = PyObject_CallObject(_currentFunction, pArgs);

//            Py_DECREF(jsonArgsPyObj);
//            Py_DECREF(pArgs);
            if (funcRes == NULL) {
//                Py_DECREF(funcRes);
                throw Php::Exception("An error during call '"+ std::string(name) + "' args:" + jsonArgs);
            }
            // Encode to JSON
            pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, funcRes);
            jsonResPyObj = PyObject_CallObject(jsonDumps, pArgs);

            jsonRes = std::string(PyUnicode_AsUTF8(jsonResPyObj));
            jsonDecodeArgs = jsonRes;
//            Py_DECREF(funcRes);
//            Py_DECREF(pArgs);
//            Py_DECREF(jsonResPyObj);
            return Php::call("json_decode", jsonDecodeArgs);
        }
        return nullptr;
    }

};

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {

/**
 *  Function that is called by PHP right after the PHP process
 *  has started, and that returns an address of an internal PHP
 *  strucure with all the details and features of your extension
 *
 *  @return void*   a pointer to an address that is understood by PHP
 */
PHPCPP_EXPORT void *get_module() {
    // static(!) Php::Extension object that should stay in memory
    // for the entire duration of the process (that's why it's static)
    static Php::Extension extension("runpy", "1.0 (https://morozov.group/en/runpy)");

    // @todo    add your own functions, classes, namespaces to the extension
//    // description of the class so that PHP knows which methods are accessible
    Php::Class <PyClass> counter("PyClass");


    counter.method<&PyClass::__construct>("__construct", {
            Php::ByVal("access_key", Php::Type::String, true),
            Php::ByVal("module", Php::Type::String, true),
            Php::ByVal("initScript", Php::Type::String, false)
    });


    extension.onStartup([]() {
        py_program = Py_DecodeLocale("runpy", NULL);
        Py_SetProgramName(py_program);  /* optional but recommended */
        Py_InitializeEx(0);
        jsonModule = PyImport_ImportModule("json");
        jsonDumps = PyObject_GetAttrString(jsonModule, "dumps");
        jsonLoads = PyObject_GetAttrString(jsonModule, "loads");
    });

    extension.onShutdown([]() {
        if (Py_FinalizeEx() < 0) {
            exit(120);
        }
        PyMem_RawFree(py_program);
    });

    //    // add the class to the extension
    extension.add(std::move(counter));

    // return the extension
    return extension;
}
}
