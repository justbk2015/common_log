#common_log
如果格式不正确，请下载这份代码，然后用记事本查看， 谢谢。

这是一个简单的c++实现的日志类。

它的主要使用类为Log,命名空间为zb

使用示例：

//************************************

// Method:    Log log的构造函数

// FullName:  zb::Log::Log

// Access:    public 

// Returns:   

// Qualifier:

// Parameter: const char * tag 此日志输出的标签名，为NULL时默认为test

// Parameter: int level  允许日志输出的等级，若设置为LOG_WARN，则该对象的debug和info不产生输出


// Parameter: int format 日志前缀包含的格式，顺序为 时间 |级别|标签|PID|  ....
// Parameter: int output 输出日志的位置，包含有控制台，文件，以及TRACE(仅windows平台有效)

// Parameter: const char * file  输出文件的位置，为NULL则重定向到控制台，为""则为dll目录下的default.log文件，可以传入绝对路径

//************************************

zb::Log log1("tag1", LOG_DEBUG, LOG_FORMAT_ALL, LOG_OUT_CF, ""); //声明一个Log对象

log1.debug("%s-%s\n", "this is a test", "test");//使用格式化接口输出

log1.info("%s-%s\n", "this is a test", "test");

log1.warn("%s-%s\n", "this is a test", "test");

log1.error("%s-%s\n", "this is a test", "test");

输出效果：

2017-01-30 00:08:32-111|DEBUG|            tag1| 4824|this is a test-test

2017-01-30 00:08:32-111| INFO|            tag1| 4824|this is a test-test

2017-01-30 00:08:32-111| WARN|            tag1| 4824|this is a test-test

2017-01-30 00:08:32-111|ERROR|            tag1| 4824|this is a test-test

//测试有中文注释
