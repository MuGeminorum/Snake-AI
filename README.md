# Snaqe
[![license](https://img.shields.io/github/license/MuGeminorum/Snake-AI.svg)](https://github.com/MuGeminorum/Snake-AI/blob/qt/LICENSE)
[![Build status](https://ci.appveyor.com/api/projects/status/y5q3wsmkp48a4yy0/branch/qt?svg=true)](https://ci.appveyor.com/project/MuGeminorum/snake-ai/branch/qt)
[![Github All Releases](https://img.shields.io/github/downloads-pre/MuGeminorum/Snake-AI/v2.1/total)](https://github.com/MuGeminorum/Snake-AI/releases/tag/v2.1)
[![](https://img.shields.io/badge/bilibili-BV1qH4y127eL-fc8bab.svg)](https://www.bilibili.com/video/BV1qH4y127eL)

基于 `A* + Hamilton` 算法、在 Qt(C++) 框架下开发的贪吃蛇 AI：它使用非机器学习的经典寻路算法 A* 和 Hamilton 通路实现贪吃蛇的自动通关。游戏本体由 C++ 版的 Qt 框架搭建，在游戏本体上进一步添加了 AI 代码。

QSnake AI: the classic snake game with AI based on `A* + Hamilton` algorithm in Qt(C++) version, how about calling it Snaqe? We use non-machine learning classical pathfinding algorithm A* and Hamilton pathway to achieve the auto-pass of Gluttony. The game body is built from a C++ version of the Qt framework, with further AI code added to the game body.

## 示例 Demo
当前算法仍然存在蛇无法吃满全屏的缺陷，还在优化避免问题示例中出现的情况：

The current algorithm still suffers from the flaw of the snake not being able to eat its way across the full screen, and is still being optimized to avoid the problems seen in the problem example:
|                                              成功示例 Success                                              |                                              问题示例 Problem                                              |
| :--------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------: |
| ![](https://user-images.githubusercontent.com/20459298/233118020-9604b2ed-c958-44c2-b27a-2c35f37948a2.gif) | ![](https://user-images.githubusercontent.com/20459298/233118070-bf508223-c7d3-4336-bff3-d21a3e169480.gif) |

## 代码下载 Code download
```bash
git clone -b qt https://github.com/MuGeminorum/Snake-AI.git
cd Snake-AI
```

## 环境安装配置及代码调试发布 Environment installation, configuration & code debugging, release
此部分内容请统一参考此[博文](https://www.cnblogs.com/MuGeminorum/p/17017055.html)

Please refer to this [blog post](https://www.cnblogs.com/MuGeminorum/p/17017063.html) for this section.
