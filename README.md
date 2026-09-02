# FreeRTOS Remote Control Microscope

基于 **STM32F429 + FreeRTOS** 开发的嵌入式远程显微镜控制系统。

项目采用多任务实时操作系统架构，将用户输入、命令处理、无线通信和状态显示等功能划分为独立任务，并通过 FreeRTOS 的任务间通信机制实现模块之间的数据传递与同步。

系统通过 **nRF24L01+** 无线模块向远程显微镜发送控制指令，实现显微镜 X/Y/Z 三轴位置、缩放（Zoom）以及亮度（Brightness）等参数的远程控制。

> This repository is intended for project documentation and portfolio demonstration only.  
> Source code from the original university assessment is not publicly provided.

---

## Project Overview

Remote Control Microscope（RCM）是一套嵌入式远程控制系统。

控制端基于 STM32F429 微控制器，通过按键、拨码开关和模拟输入获取用户指令，并利用 FreeRTOS 对系统功能进行任务化管理。

主要功能包括：

- X / Y / Z 三轴位置控制
- Zoom 缩放控制
- Brightness 亮度控制
- 无线控制指令发送
- 系统状态显示
- 命令状态机
- 数据包编码与传输
- FreeRTOS 多任务调度与任务间通信

---

## System Architecture

系统采用基于 FreeRTOS 的模块化多任务结构：

```text
                User Input
          Button / Switch / ADC
                  │
                  ▼
          ┌────────────────┐
          │ Command Input  │
          │      Task      │
          └───────┬────────┘
                  │
           Queue / Semaphore
                  │
                  ▼
          ┌────────────────┐
          │  RCM Control   │
          │      Task      │
          └───────┬────────┘
                  │
           ┌──────┴───────┐
           │              │
           ▼              ▼
 ┌────────────────┐  ┌────────────────┐
 │ Radio TX Task  │  │  Output Task   │
 └───────┬────────┘  └───────┬────────┘
         │                    │
         ▼                    ▼
    nRF24L01+           LED / RGB /
     Wireless            7-Segment
         │
         ▼
 Remote Control Microscope
