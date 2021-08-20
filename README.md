# hbapp public library
基于本项目开发的项目都叫hbapp，本项目包含了一些公共的库和一些公共的插件，方便开发者使用现在插件快速以堆积木的方式搭建项目。

### appbase
一个模块化开发框架，最大的优点就是扩展性强。使用不同的插件可以堆积木的方式搭建不同的app。引用于开源项目：https://github.com/EOSIO/appbase.git

### hb
一些公共的方法，理论上是大部分app都需要的公共方法，如日志，时间，http等。

### plugins
一些公共的插件，多个项目的交集plugin会放到public/plugins。 
使用时，开发者只需要继承本项目，实现特殊的业务plugin，与公共plugin组合成一个新的app。