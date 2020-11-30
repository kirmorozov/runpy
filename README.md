# runpy - Run Python from PHP

Module for PHP to run Python within same memory space. Using CPP Python API.
Module uses JSON serialization for messaging between PHP and Python.

`PyClass` is a class which is actually a Python module.

Methods of the `PyClass` are calling underlying Python module.

All arguments and response are serialized with JSON, please make sure arguments and result can be serialized.

Check Tensorflow example from tests. 
### Constraints
EVERY FPM/Apache worker will have its own Python Runtime, which will survive after PHP request execution.

Console script will run one PHP and one Python.

### Initialization
```PHP
<?php 
// First  argument - alias is in-memory reference to python module and whatever was initialized.
// Second argument - module to take and use for future calls;
// Third  argument - on-time preparation, will be called one if alias does not exist. 
$c = new \PyClass('__','__main__',"from time import time,ctime\n");
```

### Known limitations
Keep eye on resource utilization, Tensorflow with GPU may fail to initialize because of memo