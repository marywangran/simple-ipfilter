## 这是一个玩笑 repo，没有现实意义

封禁一个 IP 这种算小事，各个操作系统，平台都有各类机制以及配套工具完成这件事，但通过自己写程序封禁一个 IP 依然不算什么大事，用 Linux Netfilter 模块只需要不到 30 行代码，但令人震惊的是，用 Windows WFP 做同样饿事却需要 130 行代码，我 X 他大爷的，Windows 代码太多了。

我已经十几年没有摸过 Windows 系统了，而 Windows 驱动更是这辈子第一次写，不得不吐槽一下：
1. Windows API 功能细分，几乎你可以找到任何现成的 API 或其组合完成你的编程逻辑，这是好事(见下一条)；
2. Windows 做一个 mtr 简直太方便，比 Linux 封装 raw socket 简单，它有现成 IcmpSendEcho2 API，直接填表就行，不用陷入代码细节；
3. Windows 一旦出了问题，除了自己调试，几乎没有任何途径解决，因为各人各见，没人正好碰到过你这个问题，碰到了算你幸运，但这也就不再是什么疑难杂症问题了；
4. Windows 官方的代码和文档资源非常匮乏，野生资源更是门槛超级高，不是圈内人很难得到帮助，看雪有内容，但门槛高，且群体偏狂妄，几乎不愿意分享；
5. Windwos 代码多，但多数不是你自己写的，因为 Windows 重视框架；
6. Windows 问题很难获得有效帮助，但有几个精通 Windows 的朋友是最大的财富，幸运的是，我就有这样的朋友 :-)
7. 我在 Win 10 上创建的 NDIS 项目，自带的一行代码没改直接生成成功，加载失败，报错如下：
   ![image](https://github.com/user-attachments/assets/00166088-125f-402f-b6bc-ff507c92522e)
   至今未知结论，求帮助！


```bash
git clone https://github.com/marywangran/simple-ipfilter.git
```
### simple-ipfilter-Linux
```bash
cd simple-ipfilter/simple-ipfilter-Linux
make
insmod ./simple_filter.ko
ping 8.8.8.8
```
### simple-ipfilter-WINDOWS
```bash
1. cd simple-ipfilter\simple-ipfilter-WINDOWS
2. vs 20xx 打开 simple-filter.sln 并生成
3. 命令行(非 powershell)：sc create simple-filter type= kernel binPath= %你的output目录\simple-filter.sys
4. 命令行(非 powershell)：sc start simple-filter
5. ping 8.8.8.8

```
