把WTL(Windows Template Library)移植到Mingw

这是一个很久以前的项目，它使用了xushiwei的WINX Library中的ATL代码，使得WTL的移植工作简单了很多。

目前编译时还有很多警告，期待有人能完善它。

使用方法：
这是个head only库，只需要把下面三个路径加入include文件夹
atlport/include
stdext/include
wtl/include

WTL的使用方法请参考这里

