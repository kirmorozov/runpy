<?php declare(strict_types=1);
use PHPUnit\Framework\TestCase;

final class PyClassTest extends TestCase
{
    public function testTime(): void
    {
        $c = new \PyClass('__','__main__',"from time import time,ctime\n");
        $trueTime = time();
        $time = $c->time();
        $this->assertGreaterThan($trueTime, $time);
        $this->assertLessThan($trueTime+1, $time);
    }

    public function testCWD(): void
    {
        $os = new \PyClass('os','os');
        $trueCwd = getcwd();
        $pyCwd = $os->getcwd();

        $this->assertEquals($trueCwd, $pyCwd);
    }

    public function testImportModule(): void
    {
        $c = new \PyClass('_underscore_','tests.test',
        "import sys\n".
        "sys.path.append('./');\n");
        $trueTime = time();
        $underscore = $c->underscore(["hello","world"]);
        $this->assertEquals('hello_world', $underscore);
    }

    public function testTensorflow(): void
    {
        $c = new \PyClass('_tf_','tests.tf_test',
        "import sys\n".
        "sys.path.append('./');\n");
        $res = $c->predictOne([1,2,5,4,5]);
        $this->assertGreaterThan(10.01, $res[0]);
        $this->assertLessThan(10.15, $res[0]);
    }

    public function testTensorflowMany(): void
    {
        $time1 =time();
        $c = new \PyClass('_tf_','tests.tf_test',
        "import sys\n".
        "sys.path.append('./');\n");
        $res = $c->predictMany([[1,2,5,4,5], [1,2,3,5,5]]);
        $this->assertEquals(0, time()-$time1);
        $this->assertGreaterThan(10.01, $res[0][0]);
        $this->assertLessThan(10.15, $res[0][0]);
        $this->assertGreaterThan(10.01, $res[1][0]);
        $this->assertLessThan(10.15, $res[1][0]);
    }
}

