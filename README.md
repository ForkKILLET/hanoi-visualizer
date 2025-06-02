# Hanoi Visualizer

使用 C++23 编写，基于 [raylib](https://www.raylib.com/) 库的跨平台汉诺塔可视化程序。

## 背景

山东大学计算思维课大一期末作业。旨在帮助用户理解汉诺塔问题的解决过程。程序提供了一个图形化界面，用户可以通过点击按钮来控制汉诺塔的移动过程。

## 安装

可以在 [releases](https://github.com/ForkKILLET/hanoi-visualizer/releases) 页面找到自动构建的可执行文件（目前提供 linux-x86_64 和 windows-x64 两种产物）。

## 开发

本项目使用 [xmake](https://xmake.io/) 构建。安装 xmake 后，在项目根目录执行 `xmake` 命令即可构建项目。构建完成后，可以执行 `xmake run` 命令来运行程序，或在 `build` 目录下找到可执行文件。

## 项目架构

本项目的核心架构是 ECS (Entity-Component-System, 实体-组件-系统)，在 ECS 中，组件负责保存数据，系统负责处理数据，而实体本身只是一个数字 ID。这种数据驱动的架构使得程序的耦合度大大降低，便于扩展和维护。

### ECS 实现 

组件和实体是一对一关系，即每种类型的组件每个实体至多拥有一个，使得组件语义清晰、访问效率高。本项目在 `CompSet` 类中类型安全地存储同一类型的所有组件数组 `comps`，并用 `entityToIndex` 和 `indexToEntity` 记录组件和实体的对应关系。采用 SOA (structure of arrays，数组的结构) 而非 AOS (array of structures, 结构的数组)，使得内存布局连续，访问迅速。

### 组件

- `AnimationComp`：动画组件。描述基于实体所挂载组件上属性随时间变换的多轨道动画，可自定义动画曲线、快慢、方向、播放模式等。
- `BoundComp`：边界组件。描述实体的逻辑位置和大小。用于各渲染系统的渲染过程，以及点击系统的范围检测。
- `AnchorComp`：锚点组件。描述实体的带锚点位置。