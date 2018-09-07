# happy-ip
跨平台、高性能TCP&amp;UDP服务器。支持关闭延迟，支持proto的rpc，支持网络层与业务层分离，开发服务器更高效

基于boost的asio封装的高性能TCP服务器。asio已经有很好的事件封装机制，只有底层事件，没有针对TCP建立会话机制；也没有多包进行合包，以包为单位的事件提交机制。由于以上多种原因，决定对boost库进行更高的抽象和封装，对开发者提供一种更为便利的使用接口。

新特性

1、支持重叠发送数据，不会出现数据错乱问题（相关内容见AsyncWriteBuffer类）；

2、调用简单，用户不需要传递io_service；

3、接口多样，支持多种TcpServer调用；

4、服务器自带PublishOne和PublishAll方法，可指定session或者全部session发送数据；

5、自带UnorderedMapSafe的线程安全hash map，保证多线程的正确性；

6、每个事件都有response消息，当赋值时自动应答数据到客户端，非常适合经典问答模式；

7、性能突出，所有应答都是异步模式；

编译：

1、编译选项说明：

   BOOST_INCLUDE：boost头文件所在路径   
   
   CMAKE_BUILD_TYPE：编译类型（默认Debug）

2、编译命令

进入build文件夹执行如下命令

2.1 windows

cmake .. -LA -DBOOST_INCLUDE='E:/work/git' -DCMAKE_BUILD_TYPE=Debug

2.2 linux

cmake .. -LA -DBOOST_INCLUDE='/mnt/e/work/git' -DCMAKE_BUILD_TYPE=Debug

make

make install

（安装位置在install目录）
