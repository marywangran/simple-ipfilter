## 这是一个玩笑 repo，没有现实意义

封禁一个 IP 这种小事，用 Linux Netfilter 模块只需要不到 30 行代码，而用 Windows WFP 做却需要 130 行代码，我 X 他大爷的，Windows 代码太多了。

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
