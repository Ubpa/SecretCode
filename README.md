# SecretCode

## 1. 简介

用于加密代码文件，生成一个加密文件

加密流程如下，涉及 DES 加密 和 BASE64 转码

```c
// 加密
code --> line_0, line_1, ...;
defautltHeader --DES + defaultKey--> cipherDefaultHeader
userHeader --DES + userKey--> cipherUserHeader
line_i --DES + userKey--> cipherBytes_i
cipherDefaultHeader + userDefaultHeader + cipherBytes_All --BASE64--> cipher Text

// 解密
cipher Text --BASE64--> cipherDefaultHeader + cipherUserHeader + cipherBytes_All
cipherDefaultHeader --DES + defaultKey--> defautltHeader, wrong -> format error
cipherUserHeader --DES + userKey--> userHeader, wrong -> key wrong
cipherBytes_i --DES + userKey--> line_i
line_0, line_1, ... --> code
```

## 2. 编译代码

### 2.1 需求软件

- Visual Studio 2017
- Git
- CMake（版本2.8以上）

### 2.2 下载代码

```bash
git clone https://github.com/Ubpa/SecretCode
```

### 2.3 编译

1. 建立工程

```bash
# 在 cmd 中运行如下命令
mkdir build
cd build
cmake ..
```

1. 打开 `/build/SecretCode.sln`
2. 在**解决方案中**的`CMakePredefinedTargets`中，右键`INSTALL`，点击生成

## 3. 使用

在 `\bin` 中可找到生成的 `App_SecretCode.exe`

**用法**

```bash
App_SecretCode -s [in file name] (-t [out file name])? (-k [\w{8}])?
```

其中

- `-s`：源文件，后边写文件路径 
- `-t`：输出文件，后边写文件路径，**可以不填写，默认为源文件名**
- `-k`：密码，为8位字符，**可以不填写，运行程序时会询问密码，此时输入有隐藏操作**

**示例**

```bash
# 加密/解密 hello.txt, 输出覆盖 hello.txt
App_SecretCode -m e -s hello.txt -k password
# 加密/解密 hello.txt, 输出到 out.txt
App_SecretCode -m e -s hello.txt -t out.txt -k password
```

`hello.txt` 要放在 `App_SecretCode.exe` 的同目录下

**hello.txt**

```c
hello
master
```

**out.txt**

```
+/ut9cSQOn5pIEmmvMN6/m0bAFURCxFYyu1sfVTJ6e8=
+/ut9cSQOn5pIEmmvMN6/msKZ3ew7OrcoaqSTC5T18g=
NUmbtDoQDSk=
4bpFZaCQR1I=

```

