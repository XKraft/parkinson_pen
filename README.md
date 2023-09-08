# README

### 1.文件结构说明：

code文件夹：本项目的所有源码存放于该文件夹中

pico_sdk文件夹：存放pico官方提供的sdk和examples，不需要改动

raspberry-pi-pico-c-sdk.pdf：sdk的说明书

Qt: Qt工程文件夹，其中的savedata文件夹存储保存的数据

data_analysis：数据分析处理相关数据集、脚本、结果存放于此

### 2.项目开发说明：

操作系统：win10

IDE：VS Code

编程语言：C/C++

##### 具体流程：

(1)先安装相应软件

①vscode，应该都有吧

②vscode中搜索拓展插件CMake、CMake Tools下载安装

![image-20230711193106115](https://xkraft-blog-1211-1314103412.cos.ap-nanjing.myqcloud.com/img/image-20230711193106115.png)

③git，用于团队协助克隆同步代码

④gcc-arm-none-eabi交叉编译器，software文件夹里有提供，注意安装时勾选添加环境变量，不然还要自己去设置

(2)相关设置

由于我已经提前把sdk等工具包下载下来了，所有你们只需要设置相关路径变量就行，打开vs code，进入拓展选项，找到CMake Tools，对插件进行设置

![image-20230711193621779](https://xkraft-blog-1211-1314103412.cos.ap-nanjing.myqcloud.com/img/image-20230711193621779.png)

找到Configure Environment项目，设置路径变量

![image-20230711193715398](https://xkraft-blog-1211-1314103412.cos.ap-nanjing.myqcloud.com/img/image-20230711193715398.png)

找到Generator项目，设置生成器，一般来说之前写过C应该都有安装MinGW吧

![image-20230711193817246](https://xkraft-blog-1211-1314103412.cos.ap-nanjing.myqcloud.com/img/image-20230711193817246.png)

(2)打开项目

使用vs code资源管理器将code文件夹加入到工作区

![image-20230711193950567](https://xkraft-blog-1211-1314103412.cos.ap-nanjing.myqcloud.com/img/image-20230711193950567.png)

此时vs code右下角应该会问你是否要打开cmake，选择是，然后中间上方会让你选择编译器，选择之前安装的arm-none-eabi交叉编译器，至此应该可以正常跑项目了。

如果cmake运行没问题，可以对源码进行案例构建

![image-20230711194328476](https://xkraft-blog-1211-1314103412.cos.ap-nanjing.myqcloud.com/img/image-20230711194328476.png)

在build文件夹中得到的.uf2文件就是我们需要的目标文件，将其复制到树莓派pico中，树莓派就会自动加载并运行，此时树莓派会自动弹出(因为它已经不作为U盘了)，如果需要重新更换.uf2文件，按住树莓派pico上的BOOLSEL按键再重新连接到电脑，就能恢复成U盘模式。

## code工程文件夹进一步说明

主程序写在main.c中，src存放其他源文件，inc存放相应头文件，添加并使用新的源文件时，除了代码里include外，还需要在CMakeList.txt中添加相应的源文件,不然编译link时找不到源文件

![image-20230720180555606](https://xkraft-blog-1211-1314103412.cos.ap-nanjing.myqcloud.com/img/image-20230720180555606.png)