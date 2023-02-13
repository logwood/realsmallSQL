# realsmallSQL

一个Linux系统上由C++编写的数据处理系统

<!-- PROJECT SHIELDS -->

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]

<!-- PROJECT LOGO -->
<br />

<p align="center">
  <a href="https://github.com/logwood/realsmallSQL/">
    <img src="img/proj_img.png" alt="Logo" width="340" height="260">
  </a>

  <h3 align="center">REALSMALLSQL</h3>
  <p align="center">
    一个C++项目
    <br />
    <a href="https://github.com/logwood/realsmallSQL"><strong>探索本项目的文档 »</strong></a>
    <br />
    <br />
    <a href="https://github.com/logwood/realsmallSQL">查看Demo</a>
    ·
    <a href="https://github.com/logwood/realsmallSQL/issues">报告Bug</a>
    ·
    <a href="https://github.com/logwood/realsmallSQL/issues">提出新特性</a>
  </p>

</p>


 
## 目录

- [开发前的配置要求](#开发前的配置要求)
  - [安装步骤](#安装步骤)
  - [使用](#使用)
- [文件目录说明](#文件目录说明)
- [开发的架构](#开发的架构)
- [支持的功能](#支持的功能)
- [使用到的想法](#使用到的想法)
- [如何参与开源项目](#如何参与开源项目)
- [版本控制](#版本控制)
- [作者](#作者)

###### 开发前的配置要求

1. 需要带有C++20标准的GCC编译器
2. 需要UNIX系统

###### **安装步骤**


1. Clone the repo

```sh
git clone https://github.com/logwood/realsmallSQL.git
```

###### **使用**
```Shell
cd file
make
realsmallSQL
```

### 文件目录说明

```
file
├── README.md(this)
├── AVL.h
├── CreateTable.h
├── /Bplustree/
│  ├── listener.cc
|  ├── listener.h
├── /Bplustree/
│  ├── /idx/
│      └── XXX.table
|  ├── BAddTreeNode.h
|  ├── BplusFactory.h
│  └── BAddTree.h
├── delete_file.h
├── index.h
├── Makefile
├── /idx/
│      └── XXX.table
├── service.cc
├── /othertables/
│  └── XXX.table
```

### 支持的功能

#### DDL
```sql
-- 创建数据库

create database <dbname>

-- 删除数据库

drop database <dbname>

-- 创建表

create table <table-name> (
<column> <type> [primary], 
…)

-- 删除表
drop table <table-name>

```
### DML
```sql
-- 根据条件（如果有）查询表

select <column> from <table> [ where <cond> ]

-- 根据条件（如果有）删除表

delete <table> [ where <cond> ]
◼ <cond> :<column> <op> <const-value>
◼ <op> : =、<、>三者之一

-- 插入表

insert <table> values (<const-value>[, <const-value>…])

-- 插入表（2）

insert <table> into (<column>[,<column>]) values (<const-value>[, <const-value>…])
```
### DCL
```sql

-- 连接数据库，只在listener中使用。

connect (<host_ip>)

```


### 开发的架构 

请阅读[项目报告](/report.pdf) 查阅该项目的架构。

### 使用到的想法

- [B+树](https://zhuanlan.zhihu.com/p/149287061)
- [多线程远程控制](https://www.cnblogs.com/yskn/p/9355556.html)
- [AVL树](https://laravel.com)
- [B+树读写文件](https://www.cnblogs.com/cangqiongbingchen/p/4559192.html)

#### 如何参与开源项目

CONBRIBUTION使开源社区成为一个学习、激励和创造的绝佳场所。你所作的任何贡献都是**Grateful**的。


1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



### 版本控制

该项目使用Git进行版本管理。您可以在repository参看当前可用版本。

### 作者

wansanggoood@gmail.com
&ensp; 
qq:9990928  

<!-- links -->
[your-project-path]:logwood/realsmallSQL
[contributors-shield]: https://img.shields.io/github/contributors/logwood/realsmallSQL.svg?style=flat-square
[contributors-url]: https://github.com/logwood/realsmallSQL/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/logwood/realsmallSQL.svg?style=flat-square
[forks-url]: https://github.com/logwood/realsmallSQL/network/members
[stars-shield]: https://img.shields.io/github/stars/logwood/realsmallSQL.svg?style=flat-square
[stars-url]: https://github.com/logwood/realsmallSQL/stargazers
[issues-shield]: https://img.shields.io/github/issues/logwood/realsmallSQL.svg?style=flat-square
[issues-url]: https://img.shields.io/github/issues/logwood/realsmallSQL.svg
[license-shield]: https://img.shields.io/github/license/logwood/realsmallSQL.svg?style=flat-square
[license-url]: https://github.com/logwood/realsmallSQL/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/shaojintian




