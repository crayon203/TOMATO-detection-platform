# TOMATO-detection-platform
西红柿检测表组合训练推理平台-基于Qt6+YOLOv8

# TOMATO — 西红柿检测标注训练推理平台

[![Qt](https://img.shields.io/badge/Qt-6.11.0-green)](https://www.qt.io/)
[![Python](https://img.shields.io/badge/Python-3.10+-blue)](https://www.python.org/)
[![YOLOv8](https://img.shields.io/badge/YOLOv8-ultralytics-orange)](https://github.com/ultralytics/ultralytics)
[![License](https://img.shields.io/badge/License-MIT-yellow)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%2011-lightgrey)]()

面向智慧农业的桌面端西红柿目标检测一站式工具。覆盖 **图像标注 → 模型训练 → 模型量化 → 推理部署** 全链路，基于 Qt 6 + YOLOv8 + SQLite 构建，纯本地离线运行。

---

## 📸 界面预览

<!-- 截图占位：上传到 docs/screenshots/ 后取消注释
| 标注模块 | 训练模块 |
|:---:|:---:|
| ![](docs/screenshots/annot.png) | ![](docs/screenshots/train.png) |

| 推理模块 | 历史管理 |
|:---:|:---:|
| ![](docs/screenshots/infer.png) | ![](docs/screenshots/history.png) |
-->

---

## ✨ 功能概览

### 🖼 图像标注
- **五态交互状态机** — 绘制 (Drawing) / 移动 (Moving) / 缩放 (Resizing) / 平移 (Panning) / 空闲 (Idle)，统一鼠标事件，无操作冲突
- **多类别管理** — 动态增删类别，每个矩形框绑定类别标签
- **YOLO 格式标签** — 自动生成/解析 `.txt` 标签文件，与 ultralytics 训练无缝对接
- **撤销/重做** — 基于三元同步快照（框坐标 + 类别 + 置信度），支持 50 步回退
- **视图变换** — 缩放（滚轮）、旋转（按钮/输入角度）、平移（Ctrl+拖拽），适应不同尺寸图片
- **数据集浏览** — 加载文件夹 → 上一张/下一张导航 → 切换时自动保存标签 → 批量标注统计

### 🏋 模型训练
- **YOLOv8 训练** — 基于 ultralytics，支持 yolov8n/s/m/l/x 全系列
- **超参全可调** — 学习率、优化器 (SGD/Adam/AdamW)、权重衰减、验证集分割比例
- **GPU 自动检测** — CUDA 可用时自动启用 GPU，不可用时回退 CPU
- **JSON 指标回传** — 训练结束后通过 `@@METRICS_JSON@@` 标记输出结构化指标，C++ 端精确解析入库
- **实时日志** — QProcess stdout 实时回显到 GUI 日志面板
- **历史追溯** — 每次训练的超参、指标、耗时存入 SQLite，支持历史表格查询

### 📦 模型量化
- **双精度** — FP16（半精度）/ INT8（8 位整数）
- **双方案** — PTQ（训练后量化）/ QAT（感知训练量化）
- **三途径** — ONNX Runtime / TensorRT / PyTorch 多后端，失败自动回退
- **统一输出** — 所有途径最终产出可推理的 `.onnx` 文件

### 🔍 模型推理
- **双引擎** — Python YOLO 引擎 + C++ ONNX Runtime 引擎，可自由切换
- **ONNX Runtime 动态加载** — 通过 QLibrary 延迟绑定 `onnxruntime.dll`，缺失时自动降级到 Python 引擎
- **单图/批量推理** — 支持单张图片检测与文件夹批量处理
- **参数可调** — 置信度阈值 (conf)、IoU 阈值实时调节
- **结果持久化** — 每次推理的检测框坐标、置信度、类别存入数据库，支持历史查询

### 📋 数据管理
- **SQLite 五表设计** — training_records / inference_records / detection_details / categories / annotation_projects
- **DatabaseManager 单例** — 线程安全、零配置、首次运行自动建表
- **历史查询** — 训练历史表格 + 推理历史表格，支持选中行查看详情

---

## 🛠 技术栈

| 层级 | 技术 | 用途 |
|------|------|------|
| **UI 框架** | Qt 6.11.0 (Widgets) | 主窗口、标注画布、控件面板 |
| **语言** | C++17 | 业务逻辑、ONNX Runtime 推理引擎、数据库管理 |
| **深度学习** | Python 3.10+ / ultralytics (YOLOv8) | 模型训练、Python 推理、量化导出 |
| **推理加速** | ONNX Runtime (动态加载) | C++ 原生推理，零 Python 依赖 |
| **数据库** | SQLite (Qt SQL) | 训练/推理/标注记录持久化 |
| **进程通信** | QProcess + JSON (stdout 标记块) | C++ ↔ Python 跨语言数据交换 |
| **构建系统** | CMake 3.16+ | 跨平台编译 + POST_BUILD 自动部署 |
| **图像处理** | Qt QImage/QPixmap + PIL (回退) | 多格式图片加载，三层回退保底 |

---

## 📁 项目结构

```
TOMATO/
├── README.md                     # ← 本文件
├── CMakeLists.txt                # CMake 构建配置 (Qt6 + POST_BUILD 自动部署)
│
├── src/                          # C++ 源码
│   ├── main.cpp                  # 应用入口
│   ├── mainwindow.h              # 主窗口声明 (含 OnnxInference 推理引擎类)
│   ├── mainwindow.cpp            # 主窗口实现 (40+ 信号槽)
│   ├── mainwindow.ui             # Qt Designer UI 布局 (工具栏/面板/表格)
│   ├── AnnotWidget.h             # 标注控件声明 (五态状态机 + 撤销栈 + 数据集导航)
│   ├── AnnotWidget.cpp           # 标注控件实现
│   ├── database.h                # 数据库管理器声明 (单例 + 5表 CRUD)
│   └── database.cpp              # 数据库管理器实现
│
├── scripts/                      # Python 辅助脚本
│   ├── train.py                  # YOLOv8 训练 (超参可调 + JSON 指标输出)
│   ├── infer.py                  # 推理检测 (单图/批量, conf/iou 可调)
│   ├── quantize.py               # 模型量化 (PTQ/QAT, ONNX/TensorRT/PyTorch)
│   └── convert_img.py            # 图像格式转换 (PIL 回退, RGBA 原始字节流)
│
├── build/                        # 构建产物 (gitignore)
    └── Desktop_Qt_6_11_0_MinGW_64_bit-Debug/
        ├── tomatos.exe           # 编译生成的可执行文件
        ├── tomato.db             # SQLite 数据库 (首次运行自动创建)
        └── runs/detect/train*/   # 训练运行记录 (YOLO 自动生成)

---

## 🚀 构建与运行

### 环境要求

| 依赖 | 版本 | 用途 |
|------|------|------|
| Windows | 10/11 | 操作系统 |
| Qt | 6.11.0+ (MinGW 64-bit) | C++ GUI 框架 |
| CMake | 3.16+ | 构建系统 |
| Python | 3.10+ | 训练/量化/Python 推理 |
| ultralytics | 8.0.0+ | YOLOv8 深度学习库 |
| torch | 2.0.0+ | PyTorch 后端（CPU 可运行，推荐 GPU） |
| Pillow | 9.0.0+ | 图像格式转换回退 |
| onnxruntime | (可选) | C++ 推理引擎加速 |

### 安装依赖

```bash
# Python 依赖
pip install ultralytics torch pillow
```

### 构建

```bash
# 1. 配置 CMake
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug

# 2. 编译
cmake --build build

# 3. 构建完成后，POST_BUILD 自动执行：
#    - 复制 scripts/convert_img.py 到构建目录
#    - 复制 Qt imageformats 插件到构建目录
```

### 运行

```bash
cd build/Desktop_Qt_6_11_0_MinGW_64_bit-Debug
./tomatos.exe
```

首次运行后，程序目录下自动生成 `tomato.db` 数据库文件。

---

## 📝 模块速查

| 模块 | 核心文件 | 关键接口 |
|------|----------|----------|
| **标注引擎** | `src/AnnotWidget.h/cpp` | `setImage()`, `loadYoloDataset()`, `navigateToImage()`, `undo()`/`redo()` |
| **主窗口** | `src/mainwindow.h/cpp`, `ui` | `onStartTrain()`, `onRunInfer()`, `onBatchInfer()` |
| **C++ 推理** | `src/mainwindow.h:33-73` (OnnxInference) | `loadModel()`, `detect()`, `isOnnxRuntimeAvailable()` |
| **数据库** | `src/database.h/cpp` | `instance()`, `saveTrainingRecord()`, `getAllInferenceRecords()` |
| **训练脚本** | `scripts/train.py` | `--data --epochs --lr --optimizer --weight_decay --val_split` |
| **推理脚本** | `scripts/infer.py` | `--model --img --conf --iou --batch` |
| **量化脚本** | `scripts/quantize.py` | `--model --quant --scheme --pathway` |
| **格式转换** | `scripts/convert_img.py` | PIL 加载任意格式 → RGBA 原始字节流 (stdout) |

---

## 📊 性能数据

| 指标 | 实测值 |
|------|--------|
| 训练基线 mAP50 | 0.525 (YOLOv8n, 10 epochs, CPU) |
| 单 Epoch 耗时 | ~8.9 s (batch=8, imgsz=640, CPU) |
| 标注绘制帧率 | 60 FPS |
| 图片加载延迟 | < 200 ms (1920×1080 JPEG) |
| 数据集切换延迟 | < 300 ms (含自动保存标签) |
| SQLite 写入延迟 | < 50 ms (单条记录) |
| 撤销/重做 | < 5 ms (50 步栈深, 内存快照) |

训练性能可延长 epochs 至 100+ 并使用 GPU 以提升 mAP50 至 0.85+（农业部署推荐水平）。

---

## 📄 开源许可

本项目采用 [MIT License](LICENSE) 开源。

---

## 🙏 致谢

- [ultralytics](https://github.com/ultralytics/ultralytics) — YOLOv8 目标检测框架
- [Qt](https://www.qt.io/) — 跨平台 C++ GUI 框架
- [ONNX Runtime](https://onnxruntime.ai/) — 高性能推理引擎
