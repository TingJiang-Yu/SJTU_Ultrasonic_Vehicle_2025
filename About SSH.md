# SSH

这个是Deepseek生成的SSH秘钥的配置。

需要先下载git bash，这个就自行问Deepseek吧

## 什么是 SSH 密钥？

SSH 密钥是一种**安全的身份验证方式**，它使用一对加密的密钥：
- **私钥 (Private Key)**：保存在你的本地电脑上，**绝不外传**
- **公钥 (Public Key)**：可以放心地交给任何服务（如 GitHub）

就像一把锁和钥匙：
- **公钥 = 锁**：可以给别人，用来加密信息
- **私钥 = 钥匙**：自己保管，用来解密信息

## 为什么使用 SSH 而不是 HTTPS？

| 特性 | HTTPS | SSH |
|------|-------|-----|
| 身份验证 | 用户名+密码/Token | 密钥对 |
| 连接稳定性 | 可能被防火墙阻挡 | 通常更稳定 |
| 使用便利性 | 需要频繁输入凭证 | 一次配置，长期使用 |
| 安全性 | 很高 | 极高 |

## 创建 SSH 密钥的详细步骤

### 1. **检查是否已有 SSH 密钥**

打开终端（VS Code 的终端或 Git Bash），输入：
```bash
ls -al ~/.ssh
```
如果看到类似 `id_ed25519` 和 `id_ed25519.pub` 的文件，说明已有密钥。

### 2. **生成新的 SSH 密钥**

推荐使用更安全的 Ed25519 算法：
```bash
ssh-keygen -t ed25519 -C "你的邮箱地址@example.com"
```

**执行过程示例：**
```
Generating public/private ed25519 key pair.
Enter file in which to save the key (/c/Users/你的用户名/.ssh/id_ed25519): [直接回车]
Enter passphrase (empty for no passphrase): [输入密码或直接回车]
Enter same passphrase again: [再次输入密码或直接回车]
```

**说明：**
- **保存位置**：直接回车使用默认位置
- **密码**：建议设置一个安全的密码，如果嫌麻烦也可以直接回车留空

### 3. **启动 SSH 代理并添加密钥**

```bash
# 启动 SSH 代理
eval "$(ssh-agent -s)"

# 将私钥添加到代理
ssh-add ~/.ssh/id_ed25519
```

## 将公钥添加到 GitHub

### 1. **复制公钥内容**

```bash
cat ~/.ssh/id_ed25519.pub
```
**注意：** 要复制的是 `.pub` 公钥文件！

公钥内容看起来像这样：
```
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 你的邮箱地址@example.com
```

### 2. **在 GitHub 中添加公钥**

1. 登录 GitHub → 点击右上角头像 → **Settings**
2. 左侧菜单选择 **SSH and GPG keys**
3. 点击 **New SSH key**
4. 填写：
   - **Title**: 给你的密钥起个名字，如 "My Windows Laptop"
   - **Key type**: 保持默认 "Authentication Key"
   - **Key**: 粘贴刚才复制的公钥内容

5. 点击 **Add SSH key**

## 测试 SSH 连接

```bash
ssh -T git@github.com
```

如果成功，你会看到：
```
Hi TingJiang-Yu! You've successfully authenticated, but GitHub does not provide shell access.
```

## 修改仓库的远程地址为 SSH

你现在的远程地址是 HTTPS：
```
https://github.com/TingJiang-Yu/Ultrasonic_Vehicle_2025.git
```

需要改为 SSH 格式：
```
git@github.com:TingJiang-Yu/Ultrasonic_Vehicle_2025.git
```

### 修改方法：

**方法1：直接修改**
```bash
git remote set-url origin git@github.com:TingJiang-Yu/Ultrasonic_Vehicle_2025.git
```

**方法2：先查看再修改**
```bash
# 查看当前的远程地址
git remote -v

# 修改为 SSH
git remote set-url origin git@github.com:TingJiang-Yu/Ultrasonic_Vehicle_2025.git

# 再次确认修改成功
git remote -v
```

## 现在重新推送

```bash
git push origin main
```

这次应该不会再有连接错误了！

## 在 VS Code 中使用 SSH

配置完成后，在 VS Code 中：
- 所有 Git 操作（推送、拉取等）都会自动使用 SSH
- 不需要每次都输入用户名密码
- 如果设置了密钥密码，第一次操作时需要输入

## 多设备管理

如果你在多台电脑上使用 GitHub：
- 每台电脑都生成独立的 SSH 密钥对
- 在 GitHub 中为每台设备添加不同的公钥
- 可以分别命名为 "My Desktop PC", "My Laptop" 等

## 故障排除

如果 SSH 连接失败：
```bash
# 详细诊断连接问题
ssh -Tv git@github.com

# 检查密钥权限（Windows 通常不需要）
chmod 600 ~/.ssh/id_ed25519
chmod 644 ~/.ssh/id_ed25519.pub
```

**总结：** 配置 SSH 密钥一次性投入，但可以永久解决 GitHub 连接问题，特别是对于国内的网络环境来说更加稳定可靠。