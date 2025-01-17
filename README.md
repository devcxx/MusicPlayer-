# MusicPlayer+


```shell
             _                
|\/|    _o _|_)| _.   _ .__|_ 
|  ||_|_>|(_|  |(_|\/(/_|  |  
                   /          
```

本软件只用来交流测试与学习。<br>
支持的搜索和下载平台：酷狗（网易，QQ将在以后的版本中加入进来）<br>

MusicPlayer+基于[MusicPlayer2](https://github.com/zhongyang219/MusicPlayer2)二次开发，实现歌曲在线搜索、播放和下载功能。<br>
目前使用酷狗API实现歌曲搜索和下载。

### MusicPlayer2
[MusicPlayer2](https://github.com/zhongyang219/MusicPlayer2)是一款集音乐播放、歌词显示、格式转换等众多功能于一身的音频播放软件。支持歌词显示、歌词卡拉OK样式显示、歌词在线下载、歌词编辑、歌曲标签识别、专辑封面显示、专辑封面在线下载、频谱分析、音效设置、任务栏缩略图按钮、主题颜色、格式转换等功能。播放内核为BASS音频库(V2.4)。播放器支持几乎所有常见的音频格式。

### 编译
在Windows系统使用VS2015进行编译，运行时将`3rdparty\libs\x86(x64)`添加到`PATH`环境变量中或拷贝到程序运行目录

### 参考代码
- [Downloader](https://github.com/hivivo/Downloader) 基于CURL多线程下载类
- [rest-curl](https://github.com/shadow-node/rest-curl) restful curl client for napi

### 依赖库
- [bass](https://www.un4seen.com/) 音频处理
- [boost.shared_ptr](https://github.com/coldfix/boost.shared_ptr) boost shared_ptr独立版
- [fmt](https://github.com/fmtlib/fmt) 字符串格式化
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp) JSON解析
- [pthread-win32](https://github.com/GerHobbelt/pthread-win32) pthread Windows实现
> 已经将第三方库头文件、链接库和dll整理到`3rdparty`目录下

### 软件截图
主窗口：<br>
![](https://github.com/devcxx/MusicPlayerPlus/blob/master/Screenshots/Main_window.png)<br>
主窗口（隐藏播放列表）：<br>
![](https://github.com/devcxx/MusicPlayerPlus/blob/master/Screenshots/Main_window2.png)<br>

### 开发计划
- ~~x64编译环境~~
- 实现高品质音乐下载
- 整合更多音乐源（QQ、网易云）
- 使用NSIS制作安装包

### 捐赠
如果对你有用，那就支持一下吧：）<br>
![捐赠](https://github.com/devcxx/MusicPlayerPlus/blob/master/pay.png "支持一下")
