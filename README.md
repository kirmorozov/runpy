# runpy - Run Python from PHP

Module for PHP to run Python within same memory space. Using CPP Python API.
Module uses JSON serialization for messaging between PHP and Python.

## What it does?
The module is somewhat efficient solution for following questions:
1. How to call Python from within PHP?
2. Passing value from PHP script to Python script?

For complex cases where you may have massive PHP and Python code base, and want to marry them both.

### How it works

`PyClass` is a class which is actually a Python module.

Methods of the `PyClass` are calling underlying Python module.

All arguments and response are serialized with JSON, please make sure arguments and result can be serialized.

Check Tensorflow example from tests. 

### Initialization
```PHP
<?php 
// First  argument - alias is in-memory reference to python module and whatever was initialized.
// Second argument - module to take and use for future calls;
// Third  argument - on-time preparation, will be called one if alias does not exist. 
$c = new \PyClass('__','__main__',"from time import time,ctime\n");

$time = $c->time(); // call with empty array of arguments.
```

### Constraints
EVERY FPM/Apache worker will have its own Python Runtime, which will survive after PHP request execution.

Console script will run one PHP and one Python.

### Dependencies
[PHP-CPP Library](https://github.com/CopernicaMarketingSoftware/PHP-CPP) enables C++-based class for PHP and magic method calls.

[Python C API](https://docs.python.org/3.8/extending/extending.html#a-simple-example) allows communication with Python.

### Known limitations
Keep eye on resource utilization, Tensorflow with GPU may fail to initialize because of memory limits.