<?php

if (extension_loaded('runpy')) {
echo "OK\n";
}


if (class_exists('Some\PyClass')) {
echo "OK2\n";
}

$c = new \Some\Counter2();
$c->increment(2);
echo $c->value();