# GIT 版本控制

## 常用命令

| 命令                  | 功能                                                         |
| --------------------- | ------------------------------------------------------------ |
| git init              | 创建为可管理仓库                                             |
| git status            | 查看当前版本状态                                             |
| git add               | 添加内容到仓库 一般 ``git add . `` 表示全部添加                |
| git commit            | 提交到本地仓库 需要添加注释 具体为 ``git commit -m “user commit`` |
| git remote add origin | 与网络仓库绑定 ``git remote add origin git@github.com:FlameKm/test.git`` |
| git push              | 提交到网上 一般举例 ``git push origin master``                |
| git log               | 日志                                                         |
| git clone             | 克隆到本地                                                   |

## 下载

有些需要绑定, 即 ``ssh key``

使用命令

```shell
git clone
```

## 上传

[如何使用 git 命令行上传项目到 github_DreamMakers 的博客-CSDN 博客_git 新建仓库上传代码](https://blog.csdn.net/majinggogogo/article/details/81152938)

新建文件夹 test

在 test 内输入 ``git init``

![](/home/hyc/Project/StudyCode/Notes/media/image58.png)

放入代码

```shell
加全部内容
git add . 

#添加到本地仓库
git commit -m “注释内容” 

# 绑定本地仓库
git remote add origin <git@github.com:FlameKm/test.git>

# 更新
git pull --rebase origin 分支名

# 分支
git branch -M main

# 提交
git push -u origin main
```

**在 github 新建一个仓库, 与本地关联**

**似乎需要 github(或其它)ssh key 绑定本地, 否则不安全**



总结如下, GitHub 新建文件夹时也会提醒

```shell
git init
git add README.md
git commit -m "first commit"
git branch -M main
git remote add origin git@github.com:xxx.git
git push -u origin main
```

## 更新

```shell
查变更
git status

#更新全部修改到本地仓库 
git add .

#提交
git commit -m "说明信息"

#上传
git push origin master
```

## 版本回退

[Git 使用教程, 最详细，最傻瓜，最浅显，真正手把手教 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/30044692)

## 子模块 submodule

- 添加

```shell
git submodule add <https://github.com/xxx/child.git>
```

- 拉取方式一

```shell
git submodule init & git submodule update
```

- 拉取方式二

```shell
git clone --recurse-submodules <https://github.com/xxx/parent.git>
```

  也可以设置 `submodule.recurse` 为 `true`，这样每次拉去都是带这个参数的拉去 `--recurse-submodules`

- 删除：

```shell
rm -rf 子模块目录 #删除子模块目录及源码
vim .gitmodules # 删除项目目录下.gitmodules 文件中子模块相关条目
vim .git/config # 删除配置项中子模块相关条目
rm .git/modules/xxx #删除模块下的子模块目录，每个子模块对应一个目录，注意只删除对应的子模块目录即可

# 执行完成后，再执行添加子模块命令即可，如果仍然报错，执行如下：
git rm -f --cached 子模块名称
```


## 重新排除.gitignore

```shell
git rm -r --cached .
git add .
```

## 主干开发

1. 新建分支

```Shell
# 检查
git checkout main
git pull #仅主分支可用不带--base
# 新建分支
git checkout -b myfeature
```

2. 分支正常开发后提交

```Shell
git add files.c
git commit -m "[xx]: xxx"
```

3. 同步主干

```Shell
git fetch origin
```

4. 同步主干内容

```Shell
git rebase origin/main
```

5. 推送到仓库

```Shell
git push -u origin myfeature 
```

## Commit 撤销

1. 只改变注释 `git commit --amend` 
2. 撤回到上一个分支 `git reset --soft HEAD^`， soft 参数查看 RESET 章节, 

## RESET

- **--mixed**  不删除工作空间改动代码，撤销 commit，并且撤销 git add . 操作
- **--soft** 不删除工作空间改动代码，撤销 commit，不撤销 git add . 
- **--hard ** 删除工作空间改动代码，撤销 commit，撤销 git add . 
